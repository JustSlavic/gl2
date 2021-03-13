#include "object.hpp"

#include <stdio.h>


namespace object {


static const char* spaces = "                                                                       ";
static size_t print_depth = 0;
static bool use_depth = true;
static bool new_line = true;


void null_t::print () const {
    printf("null");
}


void integer_t::print () const {
    printf("%ld", value);
}


void floating_t::print () const {
    printf("%lf", value);
}


void string_t::print () const {
    printf("\"%s\"", value.c_str());
}


void boolean_t::print () const {
    printf("%s", value ? "true" : "false");
}


void object_t::print () const {
    printf("%.*s{", (new_line ? (int)print_depth : 0), spaces);

    if (use_depth) {
        printf("\n");
    } else {
        printf(" ");
    }

    print_depth += 2;
    size_t size = keys.size();
    for (size_t i = 0; i < size; i++) {
        if (use_depth)
            printf("%.*s", (int)print_depth, spaces);

        printf("%s = ", keys[i].c_str());
        new_line = false;
        values[i]->print();
        printf(";");

        if (not use_depth) {
            printf(" ");
        }

        if (use_depth) {
            printf("\n");
        }
    }
    print_depth -= 2;

    if (use_depth)
        printf("%.*s", (int)print_depth, spaces);

    printf("}");
}


void list_t::print () const {
    printf("[\n");
    new_line = true;
    print_depth += 2;

    size_t size = values.size();
    if (size > 0) {
        values[0]->print();
    }

    for (size_t i = 1; i < size; i++) {
        if (use_depth) {
            printf(",\n");
            new_line = true;
        } else {
            printf(", ");
        }

        values[i]->print();
    }

    print_depth -= 2;
    printf("\n%.*s]", (int)print_depth, spaces);
}



} // object
