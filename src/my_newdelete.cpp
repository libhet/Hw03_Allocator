#include <cstdlib>
#include <stdio.h>
#include <new>

//int debug = 1;

namespace my {

    std::size_t alloc_counter = 0;
    bool malloc_debug_output = true;

    void *malloc(std::size_t size)  throw(std::bad_alloc) {
        void *p = std::malloc(size);
        if(malloc_debug_output)
            printf("malloc: %zu %p %zu\n", ++alloc_counter, p, size);
        return p;
    }

    void free(void *p) noexcept {
        if(malloc_debug_output)
            printf("free: %zu %p\n", --alloc_counter, p);
        std::free(p);
        return;
    }
}

