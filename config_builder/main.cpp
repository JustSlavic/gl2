#include <stdio.h>
#include <string.h>
#include <son.hpp>
#include <stdarg.h>


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
        "Usage: config_builder [options] file...\n"
        "Options:\n"
        "    --output, -o                   Specify name of the output file.\n"
        "    --print, -p                    Print formatted SON object.\n"
        "    --indent [INTEGER]             Specify indent size.\n"
        "    --multiline=[auto|true|false]  Multiline mode[1].\n"
        "    --verbose, -v                  Print log messages.\n"
        "    --help, -h                     Print this message.\n"
        "\n"
        "[1]: Multiline mode defines how SON object will be printed:\n"
        "     - auto:  objects of size <= 3 and lists of size <= 5 will be printed in one line,\n"
        "other objects and lists will be printed with every entry on the new line;\n"
        "     - true:  every object entry and list element will start on the new line;\n"
        "     - false: everything will be printed in one line.\n"
        );
}


struct command_line_arguments {
    char* input_filename = nullptr;
    bool print_content = false;
    bool multiline = false;
    bool multiline_auto = true;
    int indent_size = 2;
    bool use_separator = true;
    bool verbose = false;
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

        if (strcmp(argv[i], "--indent") == 0) {
            args.indent_size = atoi(argv[++i]);
            continue;
        }

        if (strcmp(argv[i], "--multiline=true") == 0) {
            args.multiline_auto = false;
            args.multiline = true;
            continue;
        }

        if (strcmp(argv[i], "--multiline=false") == 0) {
            args.multiline_auto = false;
            args.multiline = false;
            continue;
        }

        if (strcmp(argv[i], "--multiline=auto") == 0) {
            args.multiline_auto = true;
            args.multiline = true;
            continue;
        }

        if (strcmp(argv[i], "--no-separators") == 0) {
            args.use_separator = false;
            continue;
        }

        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            args.verbose = true;
            continue;
        }

        args.input_filename = argv[i];
    }

    if (args.input_filename == nullptr) {
        printf("Please, specify input filename.\n");
        return 1;
    }

    if (args.verbose) printf("Parsing %s\n", args.input_filename);

    const size_t capacity = 10000;
    char* buffer = (char*)calloc(capacity, sizeof(char));
    size_t size = read_file(args.input_filename, buffer, capacity);

    SON::Parser parser;
    parser.verbose = args.verbose;
    parser.initialize(buffer, size, args.input_filename);

    auto* result = parser.parse();
    if (result == nullptr) {
        parser.terminate();
        free(buffer);
        return 1;
    }

    if (args.print_content) {
        auto* printer = new SON::VisitorPrint();
        printer->indent_size = args.indent_size;
        printer->multiline = args.multiline;
        printer->multiline_auto = args.multiline_auto;
        printer->use_separator = args.use_separator;
        // printer->visit(result);

        auto* scheme_visitor = new SON::VisitorIntoScheme();
        scheme_visitor->visit(result);

        printer->visit(scheme_visitor->scheme);
        printf("\n");
    }
    
    parser.terminate();
    free(buffer);

    delete result;
    return 0;
}
