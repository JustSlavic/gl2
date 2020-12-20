#pragma once

#include <vector>
#include <graphics/shader.h>

namespace service {
namespace shader {

struct ShaderLibrary {
    std::vector<Shader> library;

    ShaderLibrary();

    void initialize(const char *filename);
    void terminate();
};

} // namespace shader
} // namespace service
