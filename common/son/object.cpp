#include "object.hpp"

namespace SON {

const char* to_string (IValue::Kind k) {
    switch (k) {
        case IValue::VALUE_NULL: return "null";
        case IValue::VALUE_BOOLEAN: return "boolean";
        case IValue::VALUE_INTEGER: return "integer";
        case IValue::VALUE_FLOATING: return "floating";
        case IValue::VALUE_STRING: return "string";
        case IValue::VALUE_OBJECT: return "object";
        case IValue::VALUE_LIST: return "list";
        default: return "undefined";
    }
}

} // SON
