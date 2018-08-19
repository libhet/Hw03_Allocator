#ifndef ALLOCATOR_V_2_MY_LIST_H
#define ALLOCATOR_V_2_MY_LIST_H

#include <memory>
#include "my_allocator.h"

namespace my {
    template <typename T, typename Allocator = std::allocator<T>>
    class list{
    private:
        struct Node;
    public:
        using allocator_type = typename Allocator::template rebind<Node>::other;
        using value_type     = T;
    private:
        struct Node {

            struct NodeDeleter {
                void operator() (Node *p) {
                }
            };
            using value_type   = T;
            using node_pointer = std::unique_ptr<Node, NodeDeleter>;

            T               m_value;
            node_pointer    m_next;

        public:

            Node(): m_next(nullptr) {}

            explicit Node(const T& value)
                      : m_value(value),
                        m_next(nullptr)
            {            }

            ~Node() {

            }

            T* GetValuePointer() {return &m_value;}
            const T* GetValuePointer() const {return &m_value;}
        };

        template<typename... Args>
        typename allocator_type::pointer CreateNode(Args&&... args) {
            typename allocator_type::pointer p = GetNode();
            m_allocator.construct(p, std::forward<Args>(args)...);
            return p;
        }

        typename allocator_type::pointer GetNode() {
            return m_allocator.allocate(1);
        }

    public:
         list()  = default;
        ~list() { clear(); }

        class iterator
                : std::iterator<std::forward_iterator_tag, T>
        {
        private:
            Node *m_node = nullptr;
        public:
            iterator(Node* node): m_node(node) {}
            void operator++() {
                if(m_node) {
                    m_node = m_node->m_next.get();
                }
            }
            T & operator*() {
                if(m_node) {
                    return m_node->m_value;
                }
            }
            const T & operator*() const {
                if(m_node) {
                    return m_node->m_value;
                }
            }
            T * operator->() {
                if(m_node) {
                    return &(m_node->m_value);
                }
            }
            bool operator!=(const iterator& other) const {
                return m_node != other.m_node;
            }
            bool operator+=(const iterator& other) const {
                return !operator!=(other);
            }
        };

        iterator begin() const {
            return iterator(m_head.get());
        }
        iterator end() const {
            return iterator(nullptr);
        }


        void clear(){
            Node* current = m_head.get();
            while(current != nullptr) {
                Node* tmp = current;
                current = tmp->m_next.get();
                m_allocator.destroy(tmp);
                m_allocator.deallocate(tmp, sizeof(Node));
            }
        }

        void push_back(const T& value) {
            append(value);
        }

        void append(const T& value) {

            if(m_head.get() == nullptr) {
                m_head = std::unique_ptr<Node, typename Node::NodeDeleter>(CreateNode(value));
            }
            else {
                Node* current_node = m_head.get();
                while(current_node->m_next.get() != nullptr) {
                    current_node = current_node->m_next.get();
                }
                current_node->m_next = std::unique_ptr<Node, typename Node::NodeDeleter>(CreateNode(value));
            }
        }

    private:
        typename Node::node_pointer m_head = nullptr;
        allocator_type              m_allocator;
    };
};
#endif //ALLOCATOR_V_2_MY_LIST_H
