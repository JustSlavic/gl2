#include "reader.hpp"

namespace service {


void reader::initialize (const char* text, size_t n) {
    buffer = text;
    buffer_size = n;
    current = buffer;
}


void reader::terminate () {}


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
    auto* checkpoint = current;
    size_t count = 0;

    char c;
    while (count < n) {
        c = get_char();

        if ((c == 0) || (c != str[count])) {
            current = checkpoint;
            return result_t();
        }

        skip_char();
        count += 1;
    }

    return result_t(checkpoint, count);
}


reader::result_t reader::eat_string (const char* str)
{
    auto* checkpoint = current;
    size_t count = 0;

    char c;
    while (str[count]) {
        c = get_char();

        if ((c == 0) || (c != str[count])) {
            current = checkpoint;
            return result_t();
        }

        skip_char();
        count += 1;
    }

    return result_t(checkpoint, count);
}


} // service
