#pragma once

namespace service {

struct StringID {
    // @todo make lightweight StringID class
    // const char *id = nullptr;
    std::string id;

    StringID(const char *);
};

} // nnamespace service
