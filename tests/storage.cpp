#include <gtest/gtest.h>
#include <service/storage.hpp>



TEST(STORAGE, Create) {
    using namespace service;

    storage<size_t> s;

    for (size_t i = 0; i < 10000; i++) {
        s.push(i);
    }

    for (size_t i = 0; i < 10000; i++) {
        // printf("%ld\n", s[i]);
        s.pop();
    }

    printf("\n");
}
