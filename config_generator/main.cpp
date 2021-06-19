#include <stdio.h>
#include <son.hpp>
#include <string.h>


const char* scheme_filename     = "generated/config.scheme.son";
const char* config_hpp_filename = "generated/config.hpp";
const char* config_cpp_filename = "generated/config.cpp";


jslavic::son create_scheme_from_son(jslavic::son& value, std::string* key = nullptr) {
    using namespace jslavic;
    switch (value.type()) {
    case son::type_t::null:
    case son::type_t::boolean:
    case son::type_t::integer:
    case son::type_t::floating:
    case son::type_t::string:
        return { {"key", key ? *key : son()}, {"type", value.type_name()}, {"values", {}} };
    case son::type_t::object: {
        son result = { {"key", key ? *key : son()}, {"type", value.type_name()} };
        son values;

        for (auto p : value.pairs()) {
            auto& k = p.first;
            auto& v = p.second;
            values.push(create_scheme_from_son(v, &k));
        }

        result.push("values", values);
        return result;
    }
    case son::type_t::array: {
        // Don't do arrays yet
        return { {"key", key ? *key : son()}, {"type", value.type_name()}, {"values", {}} };
    }
    }

    return {};
}


static inline const char* son_type_to_cpp_type(const char* son_type) {
    if (strcmp(son_type, "null") == 0) {
        return "void*";
    }
    else if (strcmp(son_type, "boolean") == 0) {
        return "bool";
    }
    else if (strcmp(son_type, "integer") == 0) {
        return "int";
    }
    else if (strcmp(son_type, "floating") == 0) {
        return "double";
    }
    else if (strcmp(son_type, "string") == 0) {
        return "std::string";
    }

    return nullptr;
}


static const char* spaces = "                                                  ";
void print_cpp_config_impl(jslavic::son& value, FILE* hpp, FILE* cpp, int depth = 0, const std::string& last_variable_name = "") {
    using namespace jslavic;
    if (!value.is_object()) { return; }

    static std::vector<std::string> member_names;

    std::string key = value.get("key", "").get_string();
    std::string type = value.get("type", "").get_string();

    std::string variable_name = key == "" ? "object" : key + "_" + std::to_string(depth);

    if (key != "") member_names.push_back(key);

    if (type == "null" ||
        type == "boolean" ||
        type == "integer" ||
        type == "floating" ||
        type == "string")
    {
        fprintf(hpp, "%*.s%s %s;\n",
            depth * 4, spaces,
            son_type_to_cpp_type(value["type"].get_string().c_str()),
            key.c_str());

        fprintf(cpp, "    jslavic::son %s = %s[\"%s\"];\n", variable_name.c_str(), last_variable_name.c_str(), key.c_str());
        fprintf(cpp, "    if (!%s.is_%s()) return false;\n", variable_name.c_str(), value["type"].get_string().c_str());
        fprintf(cpp, "    cfg");
        for (auto& s : member_names) {
            fprintf(cpp, ".%s", s.c_str());
        }
        fprintf(cpp, " = %s.get_%s();\n\n", variable_name.c_str(), value["type"].get_string().c_str());
    }
    else if (value["type"] == "object")
    {
        if (key != "") {
            fprintf(cpp, "    jslavic::son %s = %s[\"%s\"];\n", variable_name.c_str(), last_variable_name.c_str(), key.c_str());
            fprintf(cpp, "    if (!%s.is_object()) return false;\n\n",
                variable_name.c_str());
        }

        if (key != "") fprintf(hpp, "%*.sstruct {\n", depth * 4, spaces);

        son& values = value["values"];

        if (values.is_array()) {
            for (auto& v : values) {
                print_cpp_config_impl(v, hpp, cpp, depth + 1, variable_name);
            }
        }

        if (key != "") fprintf(hpp, "%*.s} %s;\n", depth * 4, spaces, key.c_str());
    }
    else if (value["type"] == "array") { /* Don't do that yet. */ }

    if (key != "") member_names.pop_back();
}


void print_cpp_config_from_scheme(jslavic::son& value, const char* hpp_filename, const char* cpp_filename) {
    FILE* hpp = fopen(hpp_filename, "w");
    FILE* cpp = fopen(cpp_filename, "w");

    fprintf(hpp,
        "#pragma once\n"
        "#include <string>\n"
        "\n"
        "struct config {\n"
    );

    fprintf(cpp,
        "#include \"config.hpp\"\n"
        "#include <son.hpp>\n"
        "\n"
        "\n"
        "const config& config::get_instance() {\n"
        "    static config instance_;\n"
        "    return instance_;\n"
        "}\n"
        "\n"
        "bool config::initialize(const char* filename) {\n"
        "    jslavic::son object = jslavic::parse(filename);\n"
        "    if (object.is_null()) return false;\n"
        "    \n"
        "    config& cfg = const_cast<config&>(get_instance());\n"
        "    \n"
    );

    print_cpp_config_impl(value, hpp, cpp);

    fprintf(hpp,
        "\n"
        "    static bool initialize(const char*);\n"
        "    static const config& get_instance();\n"
        "\n"
        "private:\n"
        "    config() = default;\n"
        "    config(const config&) = delete;\n"
        "    config(config&&) = delete;\n"
        "};\n"
    );

    fprintf(cpp,
        "\n"
        "    return true;\n"
        "}\n"
    );

    fclose(cpp);
    fclose(hpp);
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please, specify input file.\n");
        return 1;
    }

    const char* input_filename = argv[1];

    jslavic::son config = jslavic::parse(input_filename);
    if (config.is_null()) {
        printf("Can't parse config file \"%s\"!", input_filename);
        return 1;
    }

    jslavic::son saved_scheme = jslavic::parse(scheme_filename);
    if (saved_scheme.is_null()) {
        printf("Could not read saved config scheme, but this is fine, we'll use our own generated scheme.\n");
        printf("Could not parse saved config scheme.\n"
            "  Although this is probably fine, if this error repeats consistently,\n"
            "  you should check scheme generator, it's probably broken.\n");
    }

    jslavic::son scheme = create_scheme_from_son(config);

    if (scheme != saved_scheme) {
        // Rewrite scheme with new one AND regenerate cpp and hpp files!
        FILE* f = fopen(scheme_filename, "w");
        if (f == nullptr) {
            printf("Could not open file \"%s\" to write scheme to.", scheme_filename);
            return 1;
        }

        jslavic::print_options options;
        options.output = f;
        jslavic::pretty_print(scheme, options);

        fclose(f);

        print_cpp_config_from_scheme(scheme, config_hpp_filename, config_cpp_filename);
    }
    else {
        printf("CONFIG BUILDER: Config schemes are equal. Exiting without generating any code.\n");
        printf("CONFIG BUILDER: Should check if config.hpp and config.cpp exist though.\n"); // @todo when os::path is finished, check if files exist.
    }

    return 0;
}
