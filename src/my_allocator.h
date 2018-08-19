#ifndef ALLOCATOR_V_2_MY_ALLOCATOR_H
#define ALLOCATOR_V_2_MY_ALLOCATOR_H

#include <memory>
#include <vector>

namespace my{

        template<typename T, size_t Capacity = 64>
        class allocator {

        public:
            using value_type = T;
            using pointer    = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference   = const T&;

            template<typename U>
            struct rebind {
                using other = allocator<U, Capacity>;
            };

        private:
            using Object_t      = std::array<unsigned char, sizeof(T)>;
            using Chunk_t       = std::array<Object_t, Capacity>;
            using ChunkPtr_t    = std::shared_ptr<Chunk_t>;

            std::vector<ChunkPtr_t> m_chunk_ptrs;
            size_t size = 0;

        private:
            inline bool     NeedBlocks()    const {
                return size % Capacity == 0;
            }
            inline void     AllocateNewBlock() {
                m_chunk_ptrs.push_back(std::make_shared<Chunk_t>());
            }
            inline size_t   BlockIdx()      const {
                return size/Capacity;
            }
            inline size_t   ElementIdx()    const {
                return size % Capacity;
            }
            inline pointer  GetPointerToElement() const {
                return reinterpret_cast<pointer>(&(m_chunk_ptrs[BlockIdx()]->operator[](ElementIdx())));
            }

        public:
             allocator() = default;
            ~allocator() = default;

            pointer allocate(size_t n = 0) {
                if(n == 1) {
                    pointer p = nullptr;
                    if (NeedBlocks()) {
                        AllocateNewBlock();
                        p = GetPointerToElement();
                        ++size;
                    } else {
                        p = GetPointerToElement();
                        ++size;
                    }
                    return p;
                } else {
                    throw std::invalid_argument( "Invalid  number of elements for allocate()" );
                }
            }

            void    deallocate  (pointer p, size_t n) {

            }

            template<typename... Args>
            void construct(T* p, Args&&... args) {
                new(p) T(std::forward<Args>(args)...);
            }

            void destroy(T* p) {
                p->~T();
            }
        };

}

#endif //ALLOCATOR_V_2_MY_ALLOCATOR_H
