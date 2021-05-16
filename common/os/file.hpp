#pragma once

#include <defines.h>
#include <string>


namespace os {


struct file {
    struct reader {
        FILE* descriptor = nullptr;

        reader () = default;

        explicit reader (const char* filename) {
            if (filename) {
                descriptor = fopen(filename, "r");
            }
        }

        reader (const reader&) = delete;

        reader (reader&& other) noexcept
            :descriptor(other.descriptor)
        {
            other.descriptor = nullptr;
        }

        ~reader () { if (descriptor) fclose(descriptor); }

        bool good () const { return descriptor != nullptr; }
    };

    struct writer {
        FILE* descriptor = nullptr;

        explicit writer (const char* filename) {
            if (filename) {
                descriptor = fopen(filename, "w");
            }
        }

        writer (const writer&) = delete;

        writer (writer&& other) noexcept
            : descriptor(other.descriptor)
        {
            other.descriptor = nullptr;
        }

        ~writer () { if (descriptor) fclose(descriptor); }

        bool good () const { return descriptor != nullptr; }

        template <typename... T>
        i32 write(T...) = delete;
    };

public:
    // path filepath;
    std::string filepath;

public:
    reader get_reader () {
        // if (not filepath.exists()) {
        //     return reader(nullptr);
        // }

        return reader(filepath.c_str());
    }

    writer get_writer () {
        // if (not filepath.exists()) {
        //     return writer(nullptr);
        // }
        return writer(filepath.c_str());
    }
};

template <> i32 file::writer::write (i32 value);

} // os
