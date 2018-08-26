#include <iostream>
#include <algorithm>
#include <map>

#include "my_allocator.h"
#include "my_list.h"


template <typename T>
void print_map(const T& container) {
    for(auto &container_member : container) {
        std::cout << container_member.first << " " << container_member.second << std::endl;
    }
}

template <typename T>
void print_container(const T &container) {
    for(auto &container_member : container) {
        std::cout << container_member << std::endl;
    }
}

constexpr auto factorial(auto n) -> decltype(n)
{
    return n ? n*factorial(n-1):1;
}

static_assert(factorial(9) == 9*8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(8) ==   8*7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(7) ==     7*6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(6) ==       6*5*4*3*2*1, "factorial failed!");
static_assert(factorial(5) ==         5*4*3*2*1, "factorial failed!");
static_assert(factorial(4) ==           4*3*2*1, "factorial failed!");
static_assert(factorial(3) ==             3*2*1, "factorial failed!");
static_assert(factorial(2) ==               2*1, "factorial failed!");
static_assert(factorial(1) ==                 1, "factorial failed!");
static_assert(factorial(0) ==                 1, "factorial failed!");


auto make_factorial_pair = [key = 0] () mutable
{
    auto value = factorial(key);
    auto factorial_pair = std::make_pair(key,value);
    ++key;
    return factorial_pair;
};


/**
 * Создание экземпляра std::map
 * Заполнение 10 элементами, где ключ это чисо от 0 до 9, а значение - факториал ключа.
 */
void create_and_fill_map() {
    std::map<int, int> std_map;

    std::generate_n(
            std::inserter(std_map,std::begin(std_map)),
            10,
            make_factorial_pair
    );
}

/**
 * Создание экземпляра std::map с новым аллокатором ограниченным 10 элементами,
 * заполнение 10 элементами, где ключ это число от 0 до 9, а значение - факториал ключа.
 * Вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере.
 */
void map_with_my_allocator() {
    std::map<int, int, std::less<int>, my::allocator<std::pair<const int, int>, 10>> std_map_with_my_allocator;

    std::generate_n(
            std::inserter(std_map_with_my_allocator,std::begin(std_map_with_my_allocator)),
            10,
            make_factorial_pair
    );

    print_map(std_map_with_my_allocator);
}

/**
 * Создание экземпляра своего контейнера для хранения int
 * заполнение 10 элементами от 0 до 9
 */
void create_and_fill_my_container() {
    my::list<int> my_list_with_std_allocator;

    for (int i = 0; i < 10; ++i) {
        my_list_with_std_allocator.append(i);
    }
}

/**
 * Создание экземпляра своего контейнера для хранения int
 * с новым аллокатором ограниченным 10 элементами,
 * заполнение 10 элементами от 0 до 9.
 * Вывод на экран всех значений хранящихся в контейнере.
 */
void my_container_with_my_allocator() {
    my::list<int, my::allocator<int, 10>> my_list_with_my_allocator;

    for (int i = 0; i < 10; ++i) {
        my_list_with_my_allocator.append(i);
    }
    print_container(my_list_with_my_allocator);
}

int main() {
    try {
        create_and_fill_map();
        map_with_my_allocator();

        create_and_fill_my_container();
        my_container_with_my_allocator();
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}