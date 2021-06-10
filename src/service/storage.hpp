#pragma once

#include <defines.hpp>
#include <stdio.h>

namespace service {


constexpr size_t STORAGE_INITIAL_SIZE = 4;

template <typename T>
struct storage {
    size_t size = 0;
    size_t capacity = 0;
    T* data = nullptr;

    storage () = default;
    ~storage () {
        if (data) {
            for (size_t i = 0; i < size; i++) {
                (data + i)->~T();
            }
            free(data);
        }
    }

    storage (const storage&) = delete;
    storage (storage&&) = delete;

    storage& operator = (const storage&) = delete;
    storage& operator = (storage&&) = delete;

    void push (T val) {
        ASSERT(capacity >= size);

        if (capacity == 0) {
            allocate();
        }

        if (size == capacity) {
            reallocate();
        }

        new (data + (size++)) T(val);
    }

    void pop () {
        ASSERT(size);

        (data + (size--))->~T();
    }

          T& operator [] (size_t idx)       { return data[idx]; }
    const T& operator [] (size_t idx) const { return data[idx]; }

private:
    void allocate() {
        ASSERT(capacity == 0);
        ASSERT(data == nullptr);
        
        capacity = STORAGE_INITIAL_SIZE;
        data = (T*) malloc(STORAGE_INITIAL_SIZE * sizeof(T));
    }

    void reallocate () {
        ASSERT(2 * capacity > capacity);
        ASSERT(data);

        T* new_storage = (T*) malloc(2 * capacity * sizeof(T));
        
        for (size_t i = 0; i < size; i++) {
            new_storage[i] = data[i];
            (data + i)->~T();
        }

        free(data);

        capacity *= 2;
        data = new_storage;
    } 
};



} // service
