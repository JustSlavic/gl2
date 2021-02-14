#include "shader_manager.hpp"
#include "str_id.hpp"

#include <stdio.h>
#include <>


namespace service {

// @warning! this function assumes that file content no longer than 1024 symbols!!!
void shader_manager::initialize (const char* filename) {
    constexpr size_t N = 1024;
    char buffer[N];

    FILE* input = fopen(filename, "r");
    if (!input) printf("File %s does not exist!\n", filename);

    int characters_read = fread(buffer, sizeof(char), N, input);

    size_t i = 0;
    while (i < characters_read) {
        const char* id_start = buffer + i;
        size_t id_len = 0;
        for (; i < characters_read && buffer[i] != '='; i++) id_len += 1;

        if (buffer[i] != '=') {
            printf("Config file \"%s\" is wrong!", filename);
        }

        str_id string_id(id_start, id_len);

        i += 1; // skip '='
        const char* filename_start = buffer + i;
        size_t filename_len = 0;
        for (; i < characters_read && buffer[i] != '\n'; i++) filename_len += 1;

        if (buffer[i] != '\n') {
            printf("Config file \"%s\" is wrong!", filename);
        }

        std::string shader_filename(filename_start, filename_len);
        i += 1; // skip '\n'

        read(shader_filename.data(), string_id);
    }
}

void shader_manager::terminate () {
    // @todo
}

void shader_manager::read (const char* filename, str_id id) {
    printf("ID: %s => SHADER: %s\n", id.id, filename);
}

} // service
