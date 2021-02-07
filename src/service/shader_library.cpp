#include "shader_library.hpp"

#include <fstream>
#include <filesystem>
#include <logging/logging.h>

namespace service {

ShaderLibrary::ShaderLibrary () {}

void ShaderLibrary::initialize (const char* filename) {
    std::ifstream input(filename);

    while (input.good() && !input.eof()) {
        char buffer[1024];
        
        input.getline(buffer, 1024);

        std::filesystem::path filepath(buffer);
        filepath = std::filesystem::current_path() / filepath; 

        LOG_DEBUG << "File: " << filepath;
        LOG_DEBUG << std::boolalpha << "exists: " << std::filesystem::exists(filepath);


    }
}

void ShaderLibrary::terminate () {}

} // service
