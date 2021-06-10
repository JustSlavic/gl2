#include <stdio.h>
#include <son.hpp>

#include <string>


size_t read_file (const char* filename, char* buffer, size_t size) {
    FILE* f = fopen(filename, "r");
    if (f == nullptr) {
        printf("Could not find file \"%s\"\n", filename);
        return 0;
    }

    size_t count = fread(buffer, sizeof(char), size, f);
    
    fclose(f);
    return count;
}


struct config_holder {
    std::string name;
    struct {
        int height;
        int width;
        std::string default_color;
    } window;

    static const config_holder& get_instance();
    static bool initialize(const char* filename);

private:
    static config_holder& get_mutable_instance();

    int parse (const char*);

    config_holder() = default;
    config_holder(const config_holder&) = delete;
    config_holder(config_holder&&) = delete;
};

static config_holder* instance = nullptr;

const config_holder& config_holder::get_instance() {
    if (not instance) instance = new config_holder();

    return *instance;
}

config_holder& config_holder::get_mutable_instance() {
    if (not instance) instance = new config_holder();

    return *instance;
}

bool config_holder::initialize(const char* filename) {
    if (not filename) return false;

    int err = get_mutable_instance().parse(filename);
    return err == 0;
}


int config_holder::parse(const char* filename) {
    const size_t capacity = 10000;
    char* buffer = (char*)calloc(capacity, sizeof(char));

    size_t size = read_file(filename, buffer, capacity);

    SON::Parser parser;
    parser.initialize(buffer, size, filename);

    SON::IValue* config_value = parser.parse();

    parser.terminate();
    free(buffer);

    config_holder& config = config_holder::get_mutable_instance();

    if (not config_value) return 1;

    SON::Object* config_object = config_value->as_object();
    if (not config_object) return 1;

    SON::IValue* name_value = config_object->get("name");
    if (not name_value) return 1;

    SON::String* name_object = name_value->as_string();
    if (not name_object) return 1;

    config.name = name_object->value;

    SON::IValue* window_value = config_object->get("window");
    if (not window_value) return 1;

    SON::Object* window_object = window_value->as_object();
    if (not window_object) return 1;

    SON::IValue* window_width_value = window_object->get("width");
    if (not window_width_value) return 1;

    SON::Integer* window_width_object = window_width_value->as_integer();
    if (not window_width_object) return 1;

    config.window.width = window_width_object->value;

    SON::IValue* window_height_value = window_object->get("height");
    if (not window_height_value) return 1;

    SON::Integer* window_height_object = window_height_value->as_integer();
    if (not window_height_object) return 1;

    config.window.height = window_height_object->value;

    SON::IValue* window_default_color_value = window_object->get("default_color");
    if (not window_default_color_value) return 1;

    SON::String* window_default_color_object = window_default_color_value->as_string();
    if (not window_default_color_object) return 1;

    config.window.default_color = window_default_color_object->value;

    return 0;
}

int main () {
    bool success = config_holder::initialize("foo.son");
    if (not success) return 1;

    const config_holder& config = config_holder::get_instance();

    printf("config.name = %s\n", config.name.c_str());
    printf("config.window.width = %d\n", config.window.width);
    printf("config.window.height = %d\n", config.window.height);
    printf("config.window.default_color = %s\n", config.window.default_color.c_str());

    return 0;
}
