#pragma once

#include <defines.h>
#include <cstring>
#include <string>
#include <vector>
#include "object.hpp"


namespace SON {


struct VisitorCounter : public IVisitor {
    using Super = IVisitor;

    int n = 0;

public:
    void visit (IValue* value) override {
        Super::visit(value);
    }

    void visit (Null* value)     override { n += 1; }
    void visit (Boolean* value)  override { n += 1; }
    void visit (Integer* value)  override { n += 1; }
    void visit (Floating* value) override { n += 1; }
    void visit (String* value)   override { n += 1; }

    void visit (const std::string& key, IValue* value) override { n += 1; this->visit(value); }

    void visit (Object* value) override {
        value->visit(this);
    }

    void visit (List* value) override {
        value->visit(this);
    }
};


struct VisitorPrint : public IVisitor {
    using Super = IVisitor;

    enum Status {
        STATUS_NONE = 0,
        STATUS_IN_OBJECT,
        STATUS_IN_LIST,
    };

    struct Settings {
        int  indent_size = 2;
        bool multiline_auto = true;
        bool multiline = true;
        bool use_separator = true;
    };

    struct State {
        Status status = STATUS_NONE;
        int depth = 0;
        int n = 0; // number of already printed pairs or list elements
        bool new_lined = true;
        bool in_kv_pair = false;
    };

    FILE*    output = stdout;
    Settings settings;
    State    state;

    const char* spaces = "                                                  "; // 50 spaces
    const int max_spaces = 50;

public:
#define print_(...) \
    { fprintf(output, __VA_ARGS__); state.new_lined = false; } void(0)

    inline void new_line () {
        if (settings.multiline) {
            fprintf(output, "\n");
            state.new_lined = true;
        }
    }

    inline void indent () {
        if (settings.multiline and state.new_lined) {
            print_("%.*s", 
                state.depth * settings.indent_size > max_spaces 
                ? max_spaces 
                : state.depth * settings.indent_size, spaces);
        }
    }

    inline const char* get_separator () {
        switch (state.status) {
            case STATUS_IN_LIST:   return state.n > 0 ? settings.multiline ? "," : ", " : ""; // if there's no elements before (n == 0), nothing to separate
            case STATUS_IN_OBJECT: return state.n > 0 ? settings.multiline ? ";" : "; " : " ";
            default: return "";
        }
    }

    inline void separator () {
        if (settings.use_separator) {
            print_("%s", get_separator());
        } else if (state.n > 0 or state.status == STATUS_IN_OBJECT) {
            print_(" ");
        }
    }

    void visit (IValue* value) override {
        if (not value) return;
        
        Super::visit(value);
    }

    void visit (Null* value) override {
        if (not value) return;
        
        if (not state.in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        state.in_kv_pair = false;
        print_("%s", to_string(value->get_kind()));
        state.n += 1;
    }

    void visit (Boolean* value) override {
        if (not value) return;
        
        if (not state.in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        state.in_kv_pair = false;
        print_("%s", value->value ? "true" : "false");
        state.n += 1;
    }

    void visit (Integer* value) override {
        if (not value) return;
        
        if (not state.in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        state.in_kv_pair = false;
        print_("%ld", value->value);
        state.n += 1;
    }

    void visit (Floating* value) override {
        if (not value) return;

        if (not state.in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        state.in_kv_pair = false;
        print_("%lf", value->value);
        state.n += 1;
    }

    void visit (String* value) override {
        if (not value) return;

        if (not state.in_kv_pair) {
            separator();
            new_line();
            indent();
        }

        state.in_kv_pair = false;
        print_("\"%s\"", value->value.c_str());
        state.n += 1;
    }

    void visit (const std::string& key, IValue* value) override {
        if (not value) return;

        separator();
        new_line();
        indent();

        print_("%s = ", key.c_str());
        state.in_kv_pair = true;
        this->visit(value);
        state.n += 1;
    }

    void visit (Object* value) override {
        if (not value) return;

        if (not state.in_kv_pair) {
            separator();
            if (state.depth > 0) new_line();
            indent();
        }

        Status previous_status = state.status;
        bool previous_multiline_setting = settings.multiline;
        int m = state.n;

        { // Count elements in the list to decide print it in one line or multiline
            VisitorCounter counter;
            counter.visit(value);

            if (settings.multiline_auto) {
                if (counter.n > 6) {
                    settings.multiline = true;
                } else {
                    settings.multiline = false;
                }
            }
        }

        { // Entering the object
            print_("{");
            state.n = 0;
            state.status = STATUS_IN_OBJECT;
            state.in_kv_pair = false;
            state.depth += 1;
        }

        value->visit(this);

        { // Leaving the object
            state.depth -= 1; // The reason for depth being decreased BEFORE printing ']' is that I want it to be on same indent as '['
            if (state.n > 0) {
                separator();
                new_line();
                indent();
            }
            print_("}");
            state.n = m + 1;
            state.status = previous_status;
            settings.multiline = previous_multiline_setting;
        }

        if (state.depth == 0) new_line();
    }

    void visit (List* value) override {
        if (not value) return;

        if (not state.in_kv_pair) {
            separator();
            if (state.depth > 0) new_line();
            indent();
        }

        Status previous_status = state.status;
        bool previous_multiline_setting = settings.multiline;
        int m = state.n;

        { // Count elements in the list to decide print it in one line or multiline
            VisitorCounter counter;
            counter.visit(value);

            if (settings.multiline_auto) {
                if (counter.n > 5) {
                    settings.multiline = true;
                } else {
                    settings.multiline = false;
                }
            }
        }

        { // Entering the list
            print_("[");
            state.n = 0;
            state.status = STATUS_IN_LIST;
            state.in_kv_pair = false;
            state.depth += 1;
        }

        value->visit(this);

        { // Leaving the list
            state.depth -= 1; // The reason for depth being decreased BEFORE printing ']' is that I want it to be on same indent as '['
            if (state.n > 0) {
                new_line();
                indent();
            }
            print_("]");
            state.n = m + 1;
            state.status = previous_status;
            settings.multiline = previous_multiline_setting;
        }

        if (state.depth == 0) new_line();
    }
#undef print_
};


struct VisitorIntoScheme : public IVisitor {
    using Super = IVisitor;

    Object* scheme = nullptr;
    List* values = nullptr;

    bool in_list = false;

public:
    virtual void visit (IValue* value) override {
        Super::visit(value);
    }

    inline void visit_value (IValue* value) {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        values->emplace(entry);
    }

    virtual void visit (Null* value)     override { visit_value(value); }
    virtual void visit (Boolean* value)  override { visit_value(value); }
    virtual void visit (Integer* value)  override { visit_value(value); }
    virtual void visit (Floating* value) override { visit_value(value); }
    virtual void visit (String* value)   override { visit_value(value); }

    virtual void visit (const std::string& key, IValue* value) override {
        if (not value) return;
        if (not values) return;

        IValue::Kind kind = value->get_kind();

        Object* entry = new Object();
        entry->emplace("key", new String(key));
        entry->emplace("type", new String(to_string(kind)));

        if (kind == IValue::VALUE_OBJECT or kind == IValue::VALUE_LIST) {
            Object* upper_scheme = scheme;
            List* upper_values = values;
            values = new List();

            in_list = true;
            value->visit(this);

            entry->emplace("values", values);
            values = upper_values;
            scheme = upper_scheme;
        }

        values->emplace(entry);
    }

    virtual void visit (Object* value) override {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        bool upper_in_list = in_list;
        in_list = false;
        List* upper_values = values;
        values = new List();

        value->visit(this);

        entry->emplace("values", values);
        values = upper_values;
        in_list = upper_in_list;

        if (in_list) {
            values->emplace(entry);
        } else {
            scheme = entry;
        }
    }

    virtual void visit (List* value) override {
        if (not value) return;

        Object* entry = new Object();
        entry->emplace("key", new Null());
        entry->emplace("type", new String(to_string(value->get_kind())));

        bool upper_in_list = in_list;
        in_list = true;

        List* upper_values = values;
        values = new List();

        value->visit(this);

        entry->emplace("values", values);

        values = upper_values;
        in_list = upper_in_list;

        if (in_list) {
            values->emplace(entry);
        } else {
            scheme = entry;
        }
    }
};


struct VisitorIntoCpp : public IVisitor {
    using Super = IVisitor;

    int indent_size = 4;
    int depth = 0;

    FILE* hpp = stdout;
    FILE* cpp = stdout;
    const char* spaces = "                                                  "; // 50 spaces
    const int max_spaces = 50;

    bool top_level = true;
    bool new_lined = false;

    int object_n = 0;
    std::vector<std::string> config_member;
    std::string up_one_level_object_name;

public:
#define print_(...) \
    { indent(); fprintf(hpp, __VA_ARGS__); new_line(); } void(0)

    inline void new_line () {
        fprintf(hpp, "\n");
        new_lined = true;
    }

    inline void indent () {
        if (new_lined) {
            fprintf(hpp, "%.*s", depth * indent_size > max_spaces ? max_spaces : depth * indent_size, spaces);
        }
    }

    void visit (IValue* value) override {
        Super::visit(value);
    }

    void print_struct (List* members) {
        if (not members) return;

        if (top_level) {
            print_("struct config {");
        } else {
            print_("struct {");
        }
        bool top = top_level;
        top_level = false;

        depth += 1;
        members->visit(this);
        depth -= 1;

        if (top) {
            fprintf(hpp,
                "\n"
                "    static const config& get_instance ();\n"
                "    static bool initialize (const char* filename);\n"
                "private:\n"
                "    config() = default;\n"
                "    config(const config&) = delete;\n"
                "    config(config&&) = delete;\n"
                );
        }

        indent();
        fprintf(hpp, "}");
    }

    void print_struct_member (Object* value) {
        String* type = value->get("type")->as_string();
        if (not type) {
            printf("Object value have no \"type\" member!\n");
            return;
        }

        std::string var_name;

        String* key = value->get("key")->as_string();

        if (key) {
            var_name = type->value + std::to_string(object_n++);
            config_member.push_back(key->value);
        } else {
            var_name = "config_value";
        }

        if (not top_level) {
            fprintf(cpp,
                "    SON::IValue* %s = %s->get(\"%s\");\n"
                "    if (not %s) return 1;\n"
                "\n",
                var_name.c_str(),
                up_one_level_object_name.c_str(),
                key ? key->value.c_str() : " ??? ",
                var_name.c_str()
            );
        }
        
        fprintf(cpp,
            "    SON::%s* %s_v = %s->as_%s();\n"
            "    if (not %s_v) return 1;\n"
            "\n",
            capitalize_type(type->value.c_str()),
            var_name.c_str(),
            var_name.c_str(),
            type->value.c_str(),
            var_name.c_str()
        );

        if (type->value != "object") {
            fprintf(cpp, "    cfg");
            for (auto name : config_member) {
                fprintf(cpp, ".%s", name.c_str());
            }
            fprintf(cpp, " = %s_v->value;\n\n", var_name.c_str());
        }

        if (type->value == "object") {
            std::string upper_object_name = up_one_level_object_name;
            up_one_level_object_name = var_name + "_v";
            print_struct(value->get("values")->as_list());  // ============= RECURSION =============
            up_one_level_object_name = upper_object_name;
        } else {
            indent();
            fprintf(hpp, "%s", son_to_cpp_type(type->value.c_str()));
        }


        if (key) {
            config_member.pop_back();
            fprintf(hpp, " %s;", key->value.c_str());
        } else {
            fprintf(hpp, ";");
        }
        new_line();
    }

    const char* son_to_cpp_type(const char* s) {
        if (strcmp(s, "boolean") == 0)  return "bool";
        if (strcmp(s, "integer") == 0)  return "int";
        if (strcmp(s, "floating") == 0) return "float";
        if (strcmp(s, "string") == 0)   return "std::string";

        return "unknown";
    }

    const char* capitalize_type(const char* s) {
        if (strcmp(s, "boolean") == 0)  return "Boolean";
        if (strcmp(s, "integer") == 0)  return "Integer";
        if (strcmp(s, "floating") == 0) return "Floating";
        if (strcmp(s, "string") == 0)   return "String";
        if (strcmp(s, "object") == 0)   return "Object";

        return "unknown";
    }

    void visit (Null* value)     override {}
    void visit (Boolean* value)  override {}
    void visit (Integer* value)  override {}
    void visit (Floating* value) override {}
    void visit (String* value)   override {}

    void visit (const std::string& key, IValue* value) override {}

    void visit (Object* value) override {
        String* type = value->get("type")->as_string();
        if (not type) {
            printf("Object value have no \"type\" member!\n");
            return;
        }

        bool top = top_level;

        // HPP
        if (top) {
            fprintf(hpp, 
                "#pragma once\n"
                "#include <string>\n"
                "\n"
                );

            fprintf(cpp,
                "#include \"config.hpp\"\n"
                "#include <son.hpp>\n"
                "\n"
                "\n"
                "static size_t read_file (const char* filename, char* buffer, size_t size) {\n"
                "    FILE* f = fopen(filename, \"r\");\n"
                "    if (f == nullptr) {\n"
                "        printf(\"Could not find file \\\"%%s\\\"\\n\", filename);\n"
                "        return 0;\n"
                "    }\n"
                "    \n"
                "    size_t count = fread(buffer, sizeof(char), size, f);\n"
                "    \n"
                "    fclose(f);\n"
                "    return count;\n"
                "}\n"
                "\n"
                "\n"
                "static config* instance = nullptr;\n"
                "\n"
                "const config& config::get_instance() {\n"
                "    if (not instance) { instance = new config(); }\n"
                "\n"
                "    return *instance;\n"
                "}\n"
                "\n"
                "static int parse_config (config& cfg, const char* filename);\n"
                "\n"
                "bool config::initialize(const char* filename) {\n"
                "    get_instance(); // Create instance if it is not created already.\n"
                "    int ec = parse_config(*instance, filename);\n"
                "    return ec == 0;\n"
                "}\n"
                "\n"
                "static int parse_config (config& cfg, const char* filename) {\n"
                "    SON::IValue* config_value = nullptr;\n"
                "    {\n"
                "        const size_t capacity = 10000;\n"
                "        char* buffer = (char*)calloc(capacity, sizeof(char));\n"
                "\n"
                "        size_t size = read_file(filename, buffer, capacity);\n"
                "\n"
                "        SON::Parser parser;\n"
                "        parser.initialize(buffer, size, filename);\n"
                "\n"
                "        config_value = parser.parse();\n"
                "\n"
                "        parser.terminate();\n"
                "        free(buffer);\n"
                "    }\n"
                "\n"
                "    if (config_value == nullptr) return 1;\n"
                "\n"
                );
        }

        print_struct_member(value);

        // CPP
        if (top) {
            fprintf(cpp,
                "    return 0;\n"
                "}\n"
                );
        }
    }

    void visit (List* value) override {
        print_("We do not support lists yet.");
    }
};


} // SON
