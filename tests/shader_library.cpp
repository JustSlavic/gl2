#include <gtest/gtest.h>

#include <service/shader_library.hpp>


TEST(ShaderLibrary, Init) {
    service::ShaderLibrary library;

    library.initialize();
}
