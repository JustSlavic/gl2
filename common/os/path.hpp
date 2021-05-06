#pragma once

#include <defines.h>


namespace os {


struct path {
    bool exists () const;
    char* c_str () const;
};


} // os
