#pragma once

#include <string>

namespace service {

struct str_id {
    const char *id = nullptr;

    str_id ();
    str_id (const char* id);
    str_id (const char* buffer, size_t len);
    str_id (const std::string& id);
    str_id (const str_id& other) = default;
};

bool operator == (const str_id& lhs, const str_id& rhs);
bool operator != (const str_id& lhs, const str_id& rhs);

} // namespace service
