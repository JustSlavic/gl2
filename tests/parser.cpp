#include <gtest/gtest.h>
#include <service/parse/lexer.hpp>
#include <service/parse/abstract_object_parser.hpp>

#include <stdio.h>


TEST (PARSER, Lexer_Spaces) {
    const char text[] = "\r   \t  \n  ";
    size_t size = sizeof(text);

    service::lexer lex;
    lex.initialize(text, size);

    lex.skip_spaces();

    lex.terminate();
}


TEST (PARSER, Lexer_Text) {
    const char text[] = 
        "{\n"
        "  screen = {\n"
        "    width = 1980;\n"
        "    height = 1080;\n"
        "  };\n"
        "};\n";

    size_t size = sizeof(text);
    
    service::lexer lex;
    lex.initialize(text, size);

    char c = lex.get_char();
    while (c != 0) {
        lex.skip_spaces();

        c = lex.eat_char();
        printf("%c", c);
    }
    printf("\n");

    lex.terminate();
}


TEST (PARSER, Abstract_Object_Parser) {
    const char text[] = 
        "{\n"
        "  screen = {\n"
        "    width = 1980;\n"
        "    height = 1080;\n"
        "  };\n"
        "};\n";

    size_t size = sizeof(text);

    service::abstract_object_parser p;

    p.initialize(text, size);

    service::abstract_object obj = p.parse();

    printf("Parsed object:\n");
    obj.print();

    p.terminate();
}
