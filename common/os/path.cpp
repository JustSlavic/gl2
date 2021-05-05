#include "path.hpp"

namespace os {

#ifdef PLATFORM_LINUX
#include "linux/path.cpp"
#endif

#ifdef PLATFORM_WINDOWS
#include "windows/path.cpp"
#endif

#ifdef PLATFORM_APPLE
#error "Path for APPLE is not implemented!"
// #include "apple/path.cpp"
#endif

} // os
