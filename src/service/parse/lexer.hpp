#pragma once

#include <defines.h>


namespace service {


struct lexer {
    struct checkpoint {
        const char* p;
        checkpoint (const char* p) :p(p) {}
    };

    size_t size = 0;
    const char* buffer = nullptr;
    const char* current = nullptr;

    lexer () = default;
    
    void initialize (const char* buffer, size_t size);
    void terminate ();

    inline char get_char () {
        ASSERT(current);
        if (current == buffer + size) { return 0; }

        return *current;
    }

    inline char eat_char () {
        ASSERT(current);
        if (current == buffer + size) { return 0; }

        return *current++;
    }

    inline void skip_char () {
        ASSERT(current);
        current++;
    }

    inline void skip_spaces () {
        char c;
        while (true) {
            c = get_char();

            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                break;
            }

            skip_char();
        }
    }

    inline const char* eat_string (const char* str, size_t size) {
        checkpoint check = make_checkpoint();

        for (size_t i = 0; i < size; i++) {
            char c = get_char();

            if (c != *str) {
                return_to(check);
                return nullptr;
            }

            skip_char();
            str++;
        }

        return check.p;
    }

    inline const checkpoint make_checkpoint () {
        return checkpoint(current);
    }

    inline void return_to(checkpoint cp) {
        current = cp.p;
    }
};


} // service
