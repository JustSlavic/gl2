#include <stdio.h>
#include <string.h>
#include <object.hpp>
#include <parse/object_parser.hpp>


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

    char buffer[10000];
    memset(buffer, 0, 10000);
    FILE* f = fopen(args.input_filename, "r");
    u64 count = fread(buffer, sizeof(char), 10000, f);
    fclose(f);

    parse::object_parser parser;
    parser.initialize(buffer, count);

    auto* result = parser.parse();
    delete result;

    parser.terminate();
    return 0;
}
