#ifndef ALLOCATOR_V_2_MY_NEWDELETE_H
#define ALLOCATOR_V_2_MY_NEWDELETE_H
#include <stdint.h>
#include <cstddef>
#include <new>
namespace my {

    extern std::size_t alloc_counter;
    extern bool malloc_debug_output;
    void* malloc(std::size_t size) throw (std::bad_alloc);
    void free(void* p) noexcept;

} // namespace my

extern "C++" {

inline void* operator new(std::size_t size) throw (std::bad_alloc)
{
    return my::malloc(size);
}

inline void operator delete(void* p) noexcept
{
    my::free(p);
}

inline void* operator new[](std::size_t size) throw (std::bad_alloc)
{
    return my::malloc(size);
}

inline void operator delete[](void* p) throw()
{
    my::free(p);
}

inline void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    return my::malloc(size);
}

inline void operator delete(void* p, const std::nothrow_t&) noexcept
{
    my::free(p);
}

inline void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    return my::malloc(size);
}

inline void operator delete[](void* p, const std::nothrow_t&) noexcept
{
    my::free(p);
}

inline void operator delete(void* p, long unsigned int)
{
    my::free(p);
}

inline void operator delete [](void* p, long unsigned int)
{
    my::free(p);
}

} // extern "C++"
#endif //ALLOCATOR_V_2_MY_NEWDELETE_H
