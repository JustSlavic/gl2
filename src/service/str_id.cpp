#include "str_id.hpp"
#include <defines.h>

#include <vector>
#include <string>

namespace service {

str_id::str_id (const char* id_) 
    : id(id_)
{}

bool operator == (const str_id& lhs, const str_id& rhs) {
    return lhs.id == rhs.id;
}

bool operator != (const str_id& lhs, const str_id& rhs) {
    return lhs.id != rhs.id;
}

} // namespace service
