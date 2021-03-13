#include <gtest/gtest.h>
#include <service/parse/reader.hpp>
#include <service/parse/abstract_object_parser.hpp>

#include <stdio.h>
#include <string.h>


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

    char buffer[10000];
    memset(buffer, 0, 10000);
    FILE* f = fopen("object.ao", "r");

    u64 count = fread(buffer, sizeof(char), 10000, f);
    printf("%ld\n", count);
    fclose(f);

    abstract_object_parser p;
    
    p.initialize(buffer, count);
    p.parse();
    p.terminate();
}
