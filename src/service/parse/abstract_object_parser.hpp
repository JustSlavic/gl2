#pragma once

#include <defines.h>
#include <service/abstract_object.hpp>


namespace service {

struct abstract_object_parser_impl;
struct abstract_object_parser {
    abstract_object_parser_impl* impl = nullptr;

    void initialize (const char* text, size_t size);
    void terminate ();

    abstract_object parse ();
};

} // service


