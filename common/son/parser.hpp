#pragma once

#include <defines.h>
#include <son.hpp>


namespace SON {


struct Parser {
    bool verbose = false;
    void* impl = nullptr;

    void initialize (const char* text, size_t size, const char* filename);
    void terminate ();

    SON::IValue* parse ();
};


} // SON


