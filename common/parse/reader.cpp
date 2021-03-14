#include "reader.hpp"


namespace parse {


void reader::initialize (const char* text, size_t n) {
    buffer = text;
    buffer_size = n;
    current = buffer;
    current_line = buffer;
}


void reader::terminate () {}


reader::result_t reader::get_line () const
{
    auto* p = current_line;
    size_t counter = 0;

    char c;
    while ((c = *p++) != 0) {
        if (is_newline(c)) { break; }
        counter += 1;
    }

    return result_t(current_line, counter);
}


reader::result_t reader::eat_while (bool (*predicate)(char))
{
    auto* checkpoint = current;
    size_t n = 0;

    char c;
    while (true) {
        c = get_char();

        if (c == 0 || not predicate(c)) { break; }

        skip_char();
        n += 1;
    }

    return result_t(checkpoint, n);
}


reader::result_t reader::eat_until (bool (*predicate)(char))
{
    auto* checkpoint = current;
    size_t count = 0;

    char c;
    while (true) {
        c = get_char();

        if (c == 0 || predicate(c)) { break; }

        skip_char();
        count += 1;
    }

    return result_t(checkpoint, count);
}


reader::result_t reader::eat_string (const char* str, size_t n)
{
    auto checkpoint = get_checkpoint();
    size_t count = 0;

    while (count < n) {
        char c = get_char();

        if ((c == 0) || (c != str[count])) {
            restore_checkpoint(checkpoint);
            return result_t();
        }

        skip_char();
        count += 1;
    }

    return result_t(checkpoint.current, count);
}


reader::result_t reader::eat_string (const char* str)
{
    auto checkpoint = get_checkpoint();
    size_t count = 0;

    while (str[count]) {
        char c = get_char();

        if ((c == 0) || (c != str[count])) {
            restore_checkpoint(checkpoint);
            return result_t();
        }

        skip_char();
        count += 1;
    }

    return result_t(checkpoint.current, count);
}


} // parse
