#include <stdio.h>
#include <son.hpp>


const char* scheme_filename     = ".generated/config.scheme.son";
const char* config_hpp_filename = ".generated/config.hpp";
const char* config_cpp_filename = ".generated/config.cpp";


size_t read_file (const char* filename, char* buffer, size_t size) {
    FILE* f = fopen(filename, "r");
    if (f == nullptr) {
        printf("Could not find file \"%s\"\n", filename);
        return 0;
    }

    size_t count = fread(buffer, sizeof(char), size, f);
    
    fclose(f);
    return count;
}


int main (int argc, char** argv) {
    if (argc < 2) {
        printf("Please, specify input file.\n");
        return 1;
    }

    const char* input_filename = argv[1];

    const size_t capacity = 10000; // @Probably a bad idea to restrain config to 10k characters!
    char* buffer = (char*)calloc(capacity, sizeof(char));

    SON::IValue* config = nullptr;
    SON::IValue* scheme = nullptr;
    SON::IValue* saved_scheme = nullptr;

    // Parse config file.
    {
        memset(buffer, 0, capacity);
        size_t size = read_file(input_filename, buffer, capacity);
        if (size == capacity) {
            printf("WARNING! Config file size exceeds buffer size = 10k characters!\n");
            return 1;
        }

        if (size == 0) {
            printf("Could not read config file! File does not exists? Check this out.\n");
            return 1;
        }

        SON::Parser parser;
        parser.initialize(buffer, size, input_filename);

        config = parser.parse();

        if (config == nullptr) {
            printf("Could not parse config! Try to fix it!\n");

            parser.terminate();
            free(buffer);
            return 1;
        }
    }

    // Try to parse already saved scheme.
    {
        memset(buffer, 0, capacity);
        size_t size = read_file(scheme_filename, buffer, capacity);
        if (size == capacity) {
            printf("WARNING! Config file size exceeds buffer size = 10k characters!\n");
            return 1;
        }

        if (size == 0) {
            printf("Could not read saved config scheme, but this is fine, we'll use our own generated scheme.\n");
        } else {
            SON::Parser parser;
            parser.initialize(buffer, size, scheme_filename);

            saved_scheme = parser.parse();

            if (saved_scheme == nullptr) {
                printf("Could not parse saved config scheme.\n"
                    "  Although this is probably fine, if this error repeats consistently,\n"
                    "  you should check scheme generator, it's probably broken.\n");
            }

            parser.terminate();
        }
    }

    // Generate new scheme from existing config.
    {
        SON::VisitorIntoScheme scheme_visitor;
        scheme_visitor.visit(config);
        scheme = scheme_visitor.scheme;
    }

    bool they_are_equal = scheme and saved_scheme and scheme->equal_to(saved_scheme);

    if (not they_are_equal) {
        // Rewrite scheme with new one AND regenerate cpp and hpp files!

        {
            FILE* f = fopen(scheme_filename, "w");
            if (f == nullptr) {
                printf("Could not open file %s. Probably folder does not exist.\n", scheme_filename);
                return 1;
            }

            SON::VisitorPrint printer;
            printer.output = f;
            printer.visit(scheme);

            fclose(f);
        }

        {
            FILE* fconfig_hpp = fopen(config_hpp_filename, "w");
            FILE* fconfig_cpp = fopen(config_cpp_filename, "w");

            if (fconfig_hpp == nullptr) {
                printf("Could not open file %s. Probably folder does not exist.\n", config_hpp_filename);
                return 1;
            }

            if (fconfig_cpp == nullptr) {
                printf("Could not open file %s. Probably folder does not exist.\n", config_cpp_filename);
                return 1;
            }

            SON::VisitorIntoCpp cpp_visitor;
            cpp_visitor.hpp = fconfig_hpp;
            cpp_visitor.cpp = fconfig_cpp;
            cpp_visitor.visit(scheme);
            
            fclose(fconfig_hpp);
            fclose(fconfig_cpp);
        }
    }
    else {
        printf("Config schemes are equal. Exiting without generating any code.\n");
        printf("Should check if config.hpp and config.cpp exist though.\n"); // @todo when os::path is finished, check if files exist.
    }

    delete config;
    delete scheme;
    delete saved_scheme;
    return 0;
}
