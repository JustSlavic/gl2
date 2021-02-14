#include <gtest/gtest.h>

#include <service/shader_manager.hpp>


TEST(Shader_Manager, Initialize_Read_Terminate) {
    service::shader_manager sm;

    sm.initialize("resources/shaders.data");
    sm.terminate();
}
