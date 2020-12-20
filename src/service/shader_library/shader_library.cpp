#include "shader_library.hpp"

#include <fstream>
#include <filesystem>
#include <logging/logging.h>

namespace service {
namespace shader {


ShaderLibrary::ShaderLibrary() {}

void ShaderLibrary::initialize(const char *filename) {
    std::ifstream input(filename);

    while (input.good() && !input.eof()) {
        char buffer[1024];
        
        input.getline(buffer, 1024);

        std::filesystem::path filepath(buffer);

        LOG_DEBUG << "File: " << std::filesystem::current_path() / filepath;
        LOG_DEBUG << std::boolalpha << "exists: " << std::filesystem::exists(std::filesystem::current_path() / filepath);
    }
}

void ShaderLibrary::terminate() {}

} // shader
} // service
