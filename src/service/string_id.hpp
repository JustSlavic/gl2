#pragma once

#include <string>

namespace service {

struct string_id {
    const char *id = nullptr;

    string_id ();
    string_id (const char* id);
    string_id (const char* buffer, size_t len);
    string_id (const std::string& id);
    string_id (const string_id& other) = default;
};

bool operator == (const string_id& lhs, const string_id& rhs);
bool operator != (const string_id& lhs, const string_id& rhs);

} // namespace service
