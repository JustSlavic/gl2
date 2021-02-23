#include <gtest/gtest.h>
#include <service/parse/reader.hpp>
#include <service/parse/abstract_object_parser.hpp>

#include <stdio.h>


TEST (PARSER, Reader) {
    using namespace service;

    const char text[] = "Hello, Sailor!\n";

    reader r;
    r.initialize(text, sizeof(text));

    reader::result_t result = r.eat_string("Hello, ");

    if (!result) {
        printf("Failed to eat string\n");
    }

    printf("%s\n", r.current);

    r.terminate();
}


TEST (PARSER, Lexer) {
    using namespace service;

    const char text[] = "{ \"key\" = truef }";

    abstract_object_parser p;
    
    p.initialize(text, sizeof(text));

    p.parse();

    p.terminate();
}
