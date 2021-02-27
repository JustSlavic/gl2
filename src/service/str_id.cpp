#include "str_id.hpp"
#include <defines.h>

#include <vector>
#include <string>
#include <cstring>
#include <iostream>


namespace service {

constexpr size_t POOL_SIZE = (1 << 10); // 1024 bytes (1 Kb)
struct str_id_pool {
    char* data = nullptr;
    char* end = nullptr;
    bool initialized = false;

    struct {
        size_t saved_ids = 0;
        size_t consumed_space = 0; // in bytes
        size_t used_space = 0;     // in bytes
    } statistics;

    ~str_id_pool() {
        terminate();
    }

    void initialize () {
        data = (char*)calloc(POOL_SIZE, sizeof(char));
        end = data;
        initialized = true;

        statistics.consumed_space = POOL_SIZE;
    }

    void terminate () {
        free(data);
        data = nullptr;
        end = nullptr;
        initialized = false;
    }

    // @Todo: Efficiency
    // @Todo: Thread-safety
    const char* create (const char* id) {
        char* word = data;
        size_t id_len = strlen(id);

        while (word != end) {
            bool found = strcmp(word, id) == 0;
            if (found) return word;
            
            word += strlen(word) + 1;
        }

        if ((u64(word) + id_len + 1) > (u64(data) + POOL_SIZE - 1)) {
            printf("%ld  bytes is not enough for requested string IDs\n", POOL_SIZE);
            ASSERT_MSG(false, "POOL_SIZE is not enough for requested string IDs");
        }
        strcpy(word, id);
        end += id_len + 1;

        statistics.saved_ids += 1;
        statistics.used_space += id_len + 1;

        printf("statistics {\n"
               "    saved_ids = %ld;\n"
               "    consumed_space = %ld bytes;\n"
               "    used_space = %ld bytes;\n"
               "}\n",
               statistics.saved_ids,
               statistics.consumed_space,
               statistics.used_space
            );

        return word;
    }
};


str_id_pool pool;


str_id::str_id () {
    if (!pool.initialized) {
        pool.initialize();
    }
}

str_id::str_id (const char* id_)
    : str_id()
{
    id = pool.create(id_);
}

str_id::str_id (const char* buffer, size_t len)
    : str_id(std::string(buffer, len))
{
}

str_id::str_id (const std::string& id_)
    : str_id()
{
    id = pool.create(id_.data());
}

bool operator == (const str_id& lhs, const str_id& rhs) {
    return lhs.id == rhs.id;
}

bool operator != (const str_id& lhs, const str_id& rhs) {
    return lhs.id != rhs.id;
}

} // namespace service
