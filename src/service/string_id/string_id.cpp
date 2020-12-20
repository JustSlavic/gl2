#include "string_id.hpp"
#include <defines.h>

#include <vector>

struct StringID_DB {
    constexpr u64 BUFFER_SIZE = 1 << 10; // KByte of IDs
    char* data = nullptr;
    char* next = nullptr;
    u64   size = 0;

    StringID_DB() {
        data = calloc(BUFFER_SIZE, sizeof(char));
        ASSERT(data);
        next = data;
    }

    ~StringID_DB() {
        free(data);
    }

    const char* make(const char* id) {
        // do smth
    }
};


static StringID_DB id_database;


StringID::StringID(const char *id_) 
    : id(id_)
{
}

