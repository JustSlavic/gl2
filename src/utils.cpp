#include <utils.h>

#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

#ifdef __linux__
#include <unistd.h>
#else
#error "This platform not supported"
#endif

#include <GL/glew.h>


std::string now_as_string() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    return std::string(buffer);
}

std::string read_whole_file(const char *filename) {
    std::ifstream input(filename, std::ios::in | std::ios::binary);
    std::ostringstream content;

    if (input.good()) {
        content << input.rdbuf();
    }

    return content.str();
}


std::string get_current_path() {
#ifdef __linux__
    // ssize_t readlink(const char *path, char *buf, size_t bufsiz);
    char* dir_name = get_current_dir_name();
    std::string result(dir_name);
    free(dir_name);
#endif

    return result;
}


bool check_gl_errors() {
    bool good = true;
    while (GLenum error = glGetError()) {
        good = false;
        switch (error) {
        case GL_INVALID_ENUM:
            printf("Error: GL_INVALID_ENUM\n"
                   "       An unacceptable value is specified for an enumerated argument.\n");
            break;
        case GL_INVALID_VALUE:
            printf("Error: GL_INVALID_VALUE\n"
                   "       A numeric argument is out of range.\n");
            break;
        case GL_INVALID_OPERATION:
            printf("Error: GL_INVALID_OPERATION\n"
                   "       The specified operation is not allowed in the current state.\n");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("Error: GL_INVALID_FRAMEBUFFER_OPERATION\n"
                   "       The framebuffer object is not complete.\n");
            break;
        case GL_OUT_OF_MEMORY:
            printf("Error: GL_OUT_OF_MEMORY\n"
                   "       There is not enough memory left to execute the command.\n"
                   "       The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
            break;
        case GL_STACK_UNDERFLOW:
            printf("Error: GL_STACK_UNDERFLOW\n"
                   "       An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
            break;
        case GL_STACK_OVERFLOW:
            printf("Error: GL_STACK_OVERFLOW\n"
                   "       An attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
            break;
        case GL_NO_ERROR:  // No error has been recorded. The value of this symbolic constant is guaranteed to be 0.
        default:
            break;
        }
    }

    return good;
}

void print_binary_repr(f32 x) {
    union {
        f32 f;
        u32 d;
    } f32_to_u32;
    f32_to_u32.f = x;

    u32 mask = 1;
    mask <<= (sizeof(u32)*8 - 1);

    while (mask) {
        printf("%d", (f32_to_u32.d & mask) > 0);
        mask >>= 1;
    }
    printf("\n");
}

void print_binary_repr(f64 x) {
    union {
        f64 f;
        u64 d;
    } f64_to_u64;
    f64_to_u64.f = x;

    u64 mask = 1;
    mask <<= (sizeof(u64)*8 - 1);

    while (mask) {
        printf("%d", (f64_to_u64.d & mask) > 0);
        mask >>= 1;
    }
    printf("\n");
}

