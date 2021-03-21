#pragma once

#include <defines.h>
#include <son.hpp>


namespace parse {

struct object_parser {
    bool verbose = false;
    void* impl = nullptr;

    void initialize (const char* text, size_t size);
    void terminate ();

    SON::IValue* parse ();
};

} // parse


