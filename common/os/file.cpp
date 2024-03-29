#include "file.hpp"

namespace os {

#ifdef PLATFORM_LINUX
#include "linux/file.cpp"
#endif

#ifdef PLATFORM_WINDOWS
#include "windows/file.cpp"
#endif

#ifdef PLATFORM_APPLE
#error "Path for APPLE is not implemented!"
// #include "apple/path.cpp"
#endif


template <>
i32 file::writer::write (i32 value) {
	ASSERT(descriptor);

	return fprintf(descriptor, "%d", value);
}


} // os
