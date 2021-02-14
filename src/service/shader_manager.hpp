#pragma once

#include "str_id.hpp"


namespace service {

struct shader_manager {
    void initialize (const char* filename);
    void terminate ();

    void read (const char* filename, str_id id);
};

} // service
