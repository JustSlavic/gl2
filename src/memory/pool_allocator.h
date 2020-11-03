#ifndef GIR2_ALLOCATOR_POOL_H
#define GIR2_ALLOCATOR_POOL_H


template<typename T, size_t N>
struct PoolAllocator {
    constexpr size_t POOL_SIZE = sizeof(T) * N; // in bytes

    void* pool = nullptr;   
};


#undef GIR2_ALLOCATOR_POOL_H
