#include "abstract_object.hpp"
#include <stdio.h>


namespace service {


static size_t print_depth = 0;
static bool use_depth = true;


void value_null::print () const {
    printf("null");
}


void value_integer::print () const {
    printf("%ld", value);
}


void value_float::print () const {
    printf("%lf", value);
}


void value_string::print () const {
    printf("\"%s\"", value.c_str());
}


void value_boolean::print () const {
    printf("%s", value ? "true" : "false");
}


void value_object::print () const {
    printf("{");

    if (use_depth) {
        printf("\n");
    } else {
        printf(" ");
    }

    print_depth += 2;
    size_t size = keys.size();
    for (size_t i = 0; i < size; i++) {
        if (use_depth)
            for (size_t j = 0; j < print_depth; ++j)
                printf(" ");

        printf("\"%s\" = ", keys[i].c_str());
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
        for (size_t j = 0; j < print_depth; ++j)
            printf(" ");

    printf("}");
}


void value_list::print () const {
    use_depth = false;
    printf("[");
    size_t size = values.size();
    if (size > 0) {
        values[0]->print();
    }

    for (size_t i = 1; i < size; i++) {
        printf(", ");
        values[i]->print();
    }
    printf("]");
    use_depth = true;
}


void abstract_object::print () const {
    if (value) {
        value->print();
    } else {
        printf("null");
    }
    printf(";\n");
}


} // service
