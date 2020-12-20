#pragma once

#include <string>

namespace service {

struct StringID {
    // @todo make lightweight StringID class
    // const char *id = nullptr;
    std::string id;

    StringID(const char *);
};

bool operator==(const StringID& lhs, const StringID& rhs);
bool operator!=(const StringID& lhs, const StringID& rhs);

} // namespace service
