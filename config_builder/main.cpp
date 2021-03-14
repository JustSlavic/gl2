#include <stdio.h>
#include <string.h>
#include <object.hpp>
#include <parse/object_parser.hpp>


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


void print_help () {
    printf(
        "usage: ./config_builder [INPUT] [KEYS]\n"
        "    Keys:\n"
        "    --output, -o     Specify name of the output file.\n"
        "    --help, -h       Print this message.\n"
        );
}


struct command_line_arguments {
    char* input_filename = nullptr;
    bool print_content = false;
};



object::object_t* create_config_signature (object::object_t* config) {
    return nullptr;
}



int main (int argc, char** argv) {
    command_line_arguments args;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_help();
            return 0;
        }

        if (strcmp(argv[i], "--print") == 0 || strcmp(argv[i], "-p") == 0) {
            args.print_content = true;
            continue;
        }

        args.input_filename = argv[i];
    }

    if (args.input_filename == nullptr) {
        printf("Please, specify input filename.\n");
        return 1;
    }

    printf("Parsing %s\n", args.input_filename);

    const size_t capacity = 10000;
    char* buffer = (char*)calloc(capacity, sizeof(char));
    size_t size = read_file(args.input_filename, buffer, capacity);

    parse::object_parser parser;
    parser.initialize(buffer, size);

    auto* result = parser.parse();
    if (result == nullptr) {
        parser.terminate();
        free(buffer);
        return 1;
    }

    if (args.print_content) {
        result->print();
        printf("\n");
    }
    
    parser.terminate();
    free(buffer);

    delete result;
    return 0;
}
