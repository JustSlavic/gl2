#include "lexer.hpp"

namespace service {


void lexer::initialize (const char* text, size_t size_) {
    buffer = text;
    size = size_;
    current = buffer;
}

void lexer::terminate () {}


} // service
