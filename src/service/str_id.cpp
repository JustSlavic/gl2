#include "str_id.hpp"
#include <defines.h>

#include <vector>
#include <string>
#include <cstring>


namespace service {

constexpr size_t POOL_SIZE = (1 << 10); // 1024 bytes (1 Kb)
struct str_id_pool {
    char* data = nullptr;
    char* end = nullptr;
    bool initialized = false;

    ~str_id_pool() {
        terminate();
    }

    void initialize () {
        data = (char*)calloc(POOL_SIZE, sizeof(char));
        end = data;
        initialized = true;
    }

    void terminate () {
        free(data);
        data = nullptr;
        end = nullptr;
        initialized = false;
    }

    // @Todo: Efficiency
    // @Todo: Thread-safe
    const char* create (const char* id) {
        char* word = data;

        while (word != end) {
            bool found = strcmp(word, id) == 0;
            if (found) return word;
            
            word += strlen(word) + 1;
        }

        if ((u64(word) + strlen(id) + 1) > (u64(data) + POOL_SIZE - 1)) {
            ASSERT(false);
        }
        strcpy(word, id);
        end += strlen(word) + 1;
        return word;
    }
};


str_id_pool pool;


str_id::str_id (const char* id_) {
    if (!pool.initialized) {
        pool.initialize();
    }

    id = pool.create(id_);
}

bool operator == (const str_id& lhs, const str_id& rhs) {
    return lhs.id == rhs.id;
}

bool operator != (const str_id& lhs, const str_id& rhs) {
    return lhs.id != rhs.id;
}

} // namespace service
