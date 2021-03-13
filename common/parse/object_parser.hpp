#pragma once

#include <defines.h>
#include <object.hpp>


namespace parse {

struct object_parser {
    void* impl = nullptr;

    void initialize (const char* text, size_t size);
    void terminate ();

    object::object_t* parse ();
};

} // parse


