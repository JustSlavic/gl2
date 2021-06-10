#pragma once

#include <defines.hpp>


namespace os {


struct path {
    bool exists () const;
    char* c_str () const;
};


} // os
