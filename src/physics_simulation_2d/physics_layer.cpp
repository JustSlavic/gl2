#include "physics_layer.hpp"
#include <config.hpp>


namespace physics_simulation_2d {


static const f32 vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};

static const u32 indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
};

constexpr f32 NEAR_CLIP_DISTANCE = 0.1f;
constexpr f32 FAR_CLIP_DISTANCE = 1000.f;


layer_world::layer_world()
    : vb(vertices, sizeof(vertices))
    , ib(indices, sizeof(indices) / sizeof(u32))
{
    auto& cfg = config::get_instance();
    f32 window_ratio = (f32)cfg.window.width / (f32)cfg.window.height;
    projection = math::projection(window_ratio * NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, FAR_CLIP_DISTANCE);

    m_camera.position = { 0, 0, 3 };

    body_shader.load_shader(Shader::Type::Vertex, "resources/shaders/body.vshader")
        .load_shader(Shader::Type::Fragment, "resources/shaders/body.fshader")
        .compile()
        .bind();

    arrow_shader.load_shader(Shader::Type::Vertex, "resources/shaders/arrow.vshader")
        .load_shader(Shader::Type::Fragment, "resources/shaders/arrow.fshader")
        .compile();

    body_shader.set_uniform_3f("u_color", math::color24::make(1.f));
    body_shader.set_uniform_mat4f("u_projection", projection);

    arrow_shader.bind();
    arrow_shader.set_uniform_mat4f("u_projection", projection);
    arrow_shader.set_uniform_3f("u_color", 0.2f, 0.5f, 0.2f);

    va.bind();
    vbl.push<f32>(3);
    va.add_buffer(vb, vbl);

    m_model.shader = &body_shader;
    m_model.arrow_shader = &arrow_shader;
    m_model.va = &va;
    m_model.ib = &ib;
}


void layer_world::draw() {
    auto view = m_camera.get_view_matrix_math();

    body_shader.set_uniform_mat4f("u_view", view);
    body_shader.set_uniform_mat4f("u_model", math::mat4::identity());

    arrow_shader.set_uniform_mat4f("u_view", view);
    arrow_shader.set_uniform_mat4f("u_model", math::mat4::identity());

	m_model.draw();
}


void layer_world::advance(f32 dt) {
	m_model.advance(dt);
}


} // physics_simulation_2d
