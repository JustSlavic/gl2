#include "creatures.hpp"
#include <graphics/renderer.h>
// #include <random>


// creature creature::make_random (math::vector2 position) {
//     std::random_device device;
//     std::mt19937 generator(device());
//     std::uniform_real_distribution<f32> size_distribution(0.01f, 0.03f);
//     std::uniform_real_distribution<f32> color_distribution(0.f, 1.f);

//     creature dude;
//     dude.position = position;
//     dude.size = size_distribution(generator);
//     dude.color.r = color_distribution(generator);
//     dude.color.g = color_distribution(generator);
//     dude.color.b = color_distribution(generator);

//     // dude.cached_model_matrix =
//     //     glm::scale(
//     //         glm::translate(
//     //             glm::mat4(1.f),
//     //             glm::vec3(position.x, position.y, 0.f)
//     //         ),
//     //         glm::vec3(dude.size));

//     return dude;
// }


void simulation::iterate() {
    draw_creatures();
}

void simulation::add_creature(creature it) {
    creatures.push_back(it);
}


void simulation::draw_creatures() {
    //for (const creature& it : creatures) {
    //    shader->set_uniform_mat4f("u_model", it.cached_model_matrix);
    //    shader->set_uniform_3f("u_color", it.color);
    //    shader->bind();
    //    gl2::Renderer::draw(*va, *ib, *shader);
    //}
}


void simulation::move_creatures(f32 dt) {

}


void simulation::clean() {
    creatures.clear();
}

