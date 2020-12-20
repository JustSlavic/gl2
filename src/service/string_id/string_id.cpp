#include "string_id.hpp"
#include <defines.h>

#include <vector>
#include <string>

namespace service {

constexpr u64 BUFFER_SIZE = 1 << 10; // KByte of IDs

struct StringID_DB {
    char* data = nullptr;
    char* next = nullptr;
    u64   size = 0;

    StringID_DB() {
        data = (char*) calloc(BUFFER_SIZE, sizeof(char));
        ASSERT(data);
        next = data;
    }

    ~StringID_DB() {
        free(data);
    }

    const char* make(const char* id) {
        // do smth
        return nullptr;
    }
};


static StringID_DB id_database;


StringID::StringID(const char *id_) 
    : id(id_)
{
}


bool operator==(const StringID& lhs, const StringID& rhs) {
    return lhs.id == rhs.id;
}

bool operator!=(const StringID& lhs, const StringID& rhs) {
    return lhs.id != rhs.id;
}

} // namespace service
