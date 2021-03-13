#pragma once

#include <defines.h>


namespace parse {


inline bool is_alpha (char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool is_digit (char c ) {
    return c >= '0' && c <= '9';
}

inline bool is_space (char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}


struct reader {
    size_t buffer_size = 0;
    const char* buffer = nullptr;
    const char* current = nullptr;

    size_t char_counter = 0;
    size_t line_counter = 0;

    struct result_t {
        const char* start = nullptr;
        size_t length = 0;

        result_t () = default;
        result_t (const char* p, size_t n) : start(p), length(n) {}

        operator bool () const { return length > 0; }
    };

    reader () = default;
    
    void initialize (const char* buffer, size_t size);
    void terminate  ();

    inline char get_char  ();
    inline char eat_char  ();
    inline void skip_char ();

    result_t eat_while (bool (*predicate)(char));
    result_t eat_until (bool (*predicate)(char));

    inline void skip_spaces () { eat_while(is_space); }

    result_t eat_string (const char* str, size_t n);
    result_t eat_string (const char* str);
};


inline char reader::get_char ()
{
    ASSERT(current);
    if (current == buffer + buffer_size) { return 0; }

    return *current;
}


inline char reader::eat_char ()
{
    ASSERT(current);
    if (current == buffer + buffer_size) { return 0; }

    return *current++;
}


inline void reader::skip_char ()
{
    eat_char();
}


} // parse
