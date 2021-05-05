#pragma once

#include <defines.h>
#include <os/path.hpp>


namespace os {


struct file {
    path filepath;

    struct reader {
        FILE* descriptor = nullptr;

        reader(const char* filename) {
            descriptor = fopen(filename, "r");
        }

        reader(const reader&) = delete;

        reader(reader&& other)
            :descriptor(other.descriptor)
        {
            other.descriptor = nullptr;
        }

        ~reader () { if (descriptor) fclose(descriptor); }

        bool good () const { return descriptor != nullptr; }

        u64 read_str (char* buffer, size_t n);

        u64 read_i8  (i8*);
        u64 read_i16 (i16*);
        u64 read_i32 (i32*);
        u64 read_i64 (i64*);

        u64 read_u8  (u8*);
        u64 read_u16 (u16*);
        u64 read_u32 (u32*);
        u64 read_u64 (u64*);

        u64 read_f32 (f32*);
        u64 read_f64 (f64*);
    };

    struct writer {
        FILE* descriptor = nullptr;

        ~writer () { if (descriptor) fclose(descriptor); }

        bool good () const { return descriptor != nullptr; }
    };

    reader get_reader () {
        return reader(filepath.c_str());
    }

    writer get_writer () {
        return writer(filepath.c_str());
    }
};


} // os
