#include "path.hpp"

#ifdef __linux__
#include "linux/path.cpp"
#endif

#ifdef _WIN32
#include "windows/path.cpp"
#endif

#ifdef __APPLE__
#error "Path for APPLE is not implemented!"
// #include "apple/path.cpp"
#endif
