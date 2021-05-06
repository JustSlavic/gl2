#pragma once

#include "string_id.hpp"


namespace service {

struct shader_manager {
    void initialize (const char* filename);
    void terminate ();

    void read (const char* filename, string_id id);
};

} // service
