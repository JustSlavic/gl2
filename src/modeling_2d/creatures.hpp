#pragma once

#include <defines.h>
#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/index_buffer.h>
#include <math.hpp>
#include <vector>


struct creature {
    enum kind_t {
        HERBIVORE,
        PREDATOR,
    };

    math::vector2 position;
    math::color24 color;
    f32 size = 0.1f;
    kind_t kind;

    glm::mat4 cached_model_matrix;

    static creature make_random (math::vector2 position);
};


struct simulation {
    std::vector<creature> creatures;

    Shader* shader = nullptr;
    VertexArray* va;
    IndexBuffer* ib;

    void iterate();

    void add_creature (creature it);
    void draw_creatures ();
    void move_creatures (f32 dt);
    void clean ();
};

