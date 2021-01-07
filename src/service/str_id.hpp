#pragma once

namespace service {

struct str_id {
    const char *id = nullptr;

    str_id (const char* id);
};

bool operator == (const str_id& lhs, const str_id& rhs);
bool operator != (const str_id& lhs, const str_id& rhs);

} // namespace service
