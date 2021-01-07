#pragma once

#include <vector>
#include <graphics/shader.h>

namespace service {

struct ShaderLibrary {
    std::vector<Shader> library;

    ShaderLibrary();

    void initialize();
    void load_from_file(const char* filename);
    void terminate();
};

} // namespace service
