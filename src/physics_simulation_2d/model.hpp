#ifndef GL2_SRC_PHYSICS_SIMULATION_2D_MODEL_HPP
#define GL2_SRC_PHYSICS_SIMULATION_2D_MODEL_HPP

#include <defines.hpp>
#include <physics/rigid_body.hpp>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/index_buffer.h>
#include <vector>


namespace physics_simulation_2d {


struct model {
    std::vector<physics::static_segment_2d> static_stuff;
    std::vector<physics::dynamic_body_2d> dynamic_stuff;

    Shader* shader = nullptr;
    Shader* arrow_shader = nullptr;
    VertexArray* va = nullptr;
    IndexBuffer* ib = nullptr;

    model();

    void draw();
    void advance(f32 dt);
};


} // physics_simulation_2d


#endif // GL2_SRC_PHYSICS_SIMULATION_2D_MODEL_HPP
