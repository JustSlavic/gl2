#ifndef GIR1_SHADER_H
#define GIR1_SHADER_H

#include <unordered_map>
#include <glm/glm.hpp>
#include <math.hpp>
#include <defines.h>


struct Shader {
    enum class Type {
        Vertex,
        Fragment
    };

    struct Uniform {
        i32 location;

        Uniform(i32);
    };

    Shader();
    Shader(const Shader&) = delete;
    Shader(Shader&&) = default;
    ~Shader();

    u32 id = 0;
    std::unordered_map<Type, std::string> sources;
    std::unordered_map<std::string, Uniform> uniform_cache;


    Shader& load_shader(Type type, const char *filename);
    Shader& compile();

    Uniform get_uniform(const char *name);

    Shader& set_uniform_1i(Uniform uniform, i32 x);
    Shader& set_uniform_1i(const char *name, i32 x);

    Shader& set_uniform_1f(Uniform uniform, f32 x);
    Shader& set_uniform_1f(const char *name, f32 x);

    Shader& set_uniform_3f(Uniform uniform, f32 x1, f32 x2, f32 x3);
    Shader& set_uniform_3f(const char *name, f32 x1, f32 x2, f32 x3);
    Shader& set_uniform_3f(const char *name, const math::vector3& value);
    Shader& set_uniform_3f(const char *name, const math::color24& value);

    Shader& set_uniform_vec3f(Uniform uniform, const glm::vec3 &vector);
    Shader& set_uniform_vec3f(const char *name, const glm::vec3 &vector);

    Shader& set_uniform_4f(Uniform uniform, f32 x1, f32 x2, f32 x3, f32 x4);
    Shader& set_uniform_4f(const char *name, f32 x1, f32 x2, f32 x3, f32 x4);

    Shader& set_uniform_mat4f(Uniform uniform, const glm::mat4& matrix);
    Shader& set_uniform_mat4f(const char *name, const glm::mat4& matrix);
    Shader& set_uniform_mat4f(Uniform uniform, const math::mat4& matrix);
    Shader& set_uniform_mat4f(const char *name, const math::mat4& matrix);

    Shader& bind();
    static void unbind();
};


#endif // GIR1_SHADER_H
