#include "layer.hpp"
#include <config.hpp>


namespace core {


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


LayerWorld::LayerWorld()
    : vb(vertices, sizeof(vertices))
    , ib(indices, sizeof(indices) / sizeof(u32))
{
    auto& cfg = config::get_instance();
    f32 window_ratio = (f32)cfg.window.width / (f32)cfg.window.height;
    projection = math::projection(window_ratio * NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, NEAR_CLIP_DISTANCE, FAR_CLIP_DISTANCE);

    Keymap2 km = load_keymap("resources/keymappings/gravity_keys.son");
    keymaps.push_back(km);

    bind<Gamepad_XBox::Event_ButtonPressed, LayerWorld, &LayerWorld::on_gamepad_button_pressed>(this);
    bind<Gamepad_XBox::Event_AxisChanged, LayerWorld, &LayerWorld::on_gamepad_axis_changed>(this);
    bind<Mouse::ButtonPressEvent, LayerWorld, &LayerWorld::on_mouse_press>(this);
    bind<Mouse::MoveEvent, LayerWorld, &LayerWorld::on_mouse_move>(this);
    bind<EventFrameFinished, LayerWorld, &LayerWorld::on_frame_finished>(this);
    
    camera.position = { 0, 0, -15 };

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
    arrow_shader.set_uniform_3f("u_color", 1.f, 1.f, 0.f);

    va.bind();
    vbl.push<f32>(3);
    va.add_buffer(vb, vbl);

    model.shader = &body_shader;
    model.va = &va;
    model.ib = &ib;
}


void LayerWorld::draw() {
    auto view = camera.get_view_matrix_math();

    body_shader.set_uniform_mat4f("u_view", view);
    body_shader.set_uniform_mat4f("u_model", math::mat4::identity());

    arrow_shader.set_uniform_mat4f("u_view", view);
    arrow_shader.set_uniform_mat4f("u_model", math::mat4::identity());

    model.draw_bodies();
}


bool LayerWorld::on_mouse_move(Mouse::MoveEvent* pEvent) {
	auto& cfg = config::get_instance();
	f32 window_ratio = (f32)cfg.window.width / (f32)cfg.window.height;

	math::vector2 mouse{
		(f32(pEvent->x) / f32(cfg.window.width) - .5f),
		(-f32(pEvent->y) / f32(cfg.window.height) + .5f)
	};

	math::vector3 position = camera.position;

	math::vector3 forward = camera.get_forward_vector();
	math::vector3 up = camera.get_up_vector();
	math::vector3 right = math::cross(forward, up);

	f32 clip_width = NEAR_CLIP_DISTANCE * window_ratio;
	f32 clip_height = NEAR_CLIP_DISTANCE;

	math::vector3 center = position + forward * NEAR_CLIP_DISTANCE;
	math::vector3 point = center + right * mouse.x * clip_width + up * mouse.y * clip_height;
	math::vector3 direction = point - position;

	math::vector3 intersection = math::intersect_z0_plane(position, direction);

	printf("mouse position: (%5.2f, %5.2f, %5.2f)\n", intersection.x, intersection.y, intersection.z);

	model.on_mouse_move(intersection.xy);

	return true;
}


bool LayerWorld::on_mouse_press(Mouse::ButtonPressEvent* pEvent) {
    if (pEvent->button == Mouse::LEFT) {
        auto& cfg = config::get_instance();
        f32 window_ratio = (f32)cfg.window.width / (f32)cfg.window.height;

        auto& m = Mouse::instance();
        // printf("==========================\n");

        math::vector3 position = camera.position;
        // printf("position = (%5.2f, %5.2f, %5.2f)\n", position.x, position.y, position.z);


        math::vector3 forward = camera.get_forward_vector();
        math::vector3 up = camera.get_up_vector();
        math::vector3 right = math::cross(forward, up);

        // f32 t = 2 * NEAR_CLIP_DISTANCE * ::std::tan(.5f * math::radians(45.f));
        // f32 clip_width = t * window_ratio;
        // f32 clip_height = t;

        f32 clip_width = NEAR_CLIP_DISTANCE * window_ratio;
        f32 clip_height = NEAR_CLIP_DISTANCE;

        // printf("NEAR_CLIP{ WIDTH = %5.2f; HEIGHT = %5.2f; }\n", clip_width, clip_height);

        math::vector2 mouse = {
            (f32(m.x) / f32(cfg.window.width) - .5f),
            (-f32(m.y) / f32(cfg.window.height) + .5f)
        };
        // printf("mouse = (%5.2f, %5.2f)\n", mouse.x, mouse.y);

        math::vector3 center = position + forward * NEAR_CLIP_DISTANCE;
        math::vector3 point = center + right * mouse.x * clip_width + up * mouse.y * clip_height;
        // printf("point = (%5.2f, %5.2f, %5.2f)\n", point.x, point.y, point.z);

        math::vector3 direction = point - position;

        math::vector3 intersection = math::intersect_z0_plane(position, direction);

        printf("click position: (%5.2f, %5.2f, %5.2f)\n", intersection.x, intersection.y, intersection.z);

        math::mat4 model_ = math::mat4::identity();
        math::mat4 view_ = camera.get_view_matrix_math();

        math::vector4 q = math::vector4::make(intersection, 0.f);

        q = model_ * q;
        //printf("in space: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);
        q = q * view_;
        //printf("in camera: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);
        q = projection * q;
        //printf("in screen: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);

        q = q / q.w;
        //printf("normalized: q = (%5.2f, %5.2f, %5.2f, %5.2f)\n", q.x, q.y, q.z, q.w);

        model.on_left_mouse_click(intersection.xy);
    }
    else if (pEvent->button == Mouse::RIGHT) {
        model.on_right_mouse_click({ 0.f });
    }

    return true;
}

} // core

