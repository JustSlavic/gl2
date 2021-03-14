#include "object.hpp"

#include <stdio.h>


namespace object {


static const char* spaces = "                                                                       ";
static int indent = 0;
static bool use_indent = true;
static bool new_line = true;


void print_indent() {
    printf("%.*s", indent, spaces);
}


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
    printf("{");

    size_t size = keys.size();
    if (size > 0) {
        if (use_indent) {
            printf("\n");
            new_line = true;
        } else {
            printf(" ");
        }
    }

    indent += 2;
    for (size_t i = 0; i < size; i++) {
        if (use_indent)
            printf("%.*s", indent, spaces);

        printf("%s = ", keys[i].c_str());
        new_line = false;
        values[i]->print();
        printf(";");

        if (not use_indent) {
            printf(" ");
        }

        if (use_indent) {
            printf("\n");
            new_line = true;
        }
    }
    indent -= 2;

    if (use_indent and size > 0)
        printf("%.*s", indent, spaces);

    printf("}");
}


void list_t::print () const {
    printf("[");
    new_line = true;
    indent += 2;

    size_t size = values.size();
    if (size > 0) {
        printf("\n");
        if (use_indent) {
            print_indent();
        }
        values[0]->print();
    }

    for (size_t i = 1; i < size; i++) {
            printf(",");
        if (use_indent) {
            printf("\n");
            print_indent();
        } else {
            printf(" ");
        }

        values[i]->print();
    }

    if (use_indent and size > 0) {
        printf("\n");
    }

    indent -= 2;
    printf("%.*s]", (use_indent and size > 0 ? indent : 0), spaces);
}



} // object
