#define BOOST_TEST_MODULE TEST_ALLOCATOR

#include "../src/my_newdelete.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../src/my_allocator.h"

#include <iostream>
#include <list>
#include <map>
#include <iterator>

#include "../src/my_list.h"

struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
            : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::streambuf * old;
};

struct MyTestStruct {
    MyTestStruct(int value, bool destructor_debug = false)
            : a(value), b(value), c(value), d(destructor_debug){}
    ~MyTestStruct()     {if (d) std::cout << "deleted " << a << std::endl;}
    int     a = 1;
    float   b = 2.0f;
    double  c = 3.0;
    bool    d = false;
};

auto getMTS = [i = 0]() mutable {
    return i++;
};

BOOST_AUTO_TEST_SUITE(Allocator)

    BOOST_AUTO_TEST_CASE(CanCreate) {
        my::allocator<int> allocator;
        my::allocator<MyTestStruct> allocator_my_struct;
    }

    BOOST_AUTO_TEST_CASE(allocate_1) {
        my::allocator<MyTestStruct> allocator_my_struct;
        MyTestStruct *pointer = allocator_my_struct.allocate(1);
        BOOST_CHECK(pointer);
    }

    BOOST_AUTO_TEST_CASE(construct) {
        my::allocator<MyTestStruct> allocator_my_struct;
        MyTestStruct *pointer = allocator_my_struct.allocate(1);
        allocator_my_struct.construct(pointer, 5);

        BOOST_CHECK(pointer->a == 5);
        BOOST_CHECK(pointer->b == 5.0f);
        BOOST_CHECK(pointer->c == 5.0);
    }

    BOOST_AUTO_TEST_CASE(destroy) {

        boost::test_tools::output_test_stream output;
        cout_redirect guard(output.rdbuf());

        my::allocator<MyTestStruct> allocator_my_struct;
        MyTestStruct *pointer = allocator_my_struct.allocate(1);
        allocator_my_struct.construct(pointer, 5, true);

        BOOST_CHECK(pointer->a == 5);
        BOOST_CHECK(pointer->b == 5.0f);
        BOOST_CHECK(pointer->c == 5.0);

        allocator_my_struct.destroy(pointer);

        BOOST_CHECK(output.is_equal("deleted 5\n"));
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ALLOCATOR_WITH_STD_LIST)

    BOOST_AUTO_TEST_CASE(CreateAndRead_SeveralElements) {
        std::list<MyTestStruct, my::allocator<MyTestStruct, 10>> list;

        std::generate_n(std::back_inserter(list), 10, getMTS
        );

        {
            boost::test_tools::output_test_stream output;
            cout_redirect guard(output.rdbuf());

            for (auto &element : list) {
                std::cout << element.a << std::endl;
            }

            BOOST_CHECK(output.is_equal("0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n"));
        }
    }

    BOOST_AUTO_TEST_CASE(CreateAndRead_SeveralElements_WithReallocate) {
        /// Chunk of allocator contains only two elements
        std::list<MyTestStruct, my::allocator<MyTestStruct, 2>> list;

        auto getMTS = [i = 0]() mutable {
            return i++;
        };

        std::generate_n(std::back_inserter(list), 10, getMTS
        );

        {
            boost::test_tools::output_test_stream output;
            cout_redirect guard(output.rdbuf());

            for (auto &element : list) {
                std::cout << element.a << std::endl;
            }

            BOOST_CHECK(output.is_equal("0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n"));
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ALLOCATOR_WITH_STD_MAP)
    BOOST_AUTO_TEST_CASE(Compare_MemoryAllocations_With_StdAllocator) {

        size_t alloc_before = my::alloc_counter;
        size_t alloc_after_std, alloc_after_my;

        auto sq = [i = 0]() mutable {
            auto r = std::make_pair(i, i*i);
            i++;
            return r;
        };
        {
            std::map<int, int> std_map;
            std::generate_n(
                    std::inserter(std_map,std::begin(std_map)),
                    10,
                    sq
            );
            alloc_after_std = my::alloc_counter;
        }

        {
            std::map<int, int, std::less<int>, my::allocator<std::pair<const int, int>, 10>> my_map;
            std::generate_n(
                    std::inserter(my_map,std::begin(my_map)),
                    10,
                    sq
            );
            alloc_after_my = my::alloc_counter;
        }
        alloc_after_std -= alloc_before;
        alloc_after_my  -= alloc_before;

        std::cout << "std::allocator : " << alloc_after_std << std::endl;
        std::cout << "my::allocator  : " << alloc_after_my << std::endl;

        BOOST_CHECK(alloc_after_my < alloc_after_std);
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MyContainer)
    BOOST_AUTO_TEST_CASE(CreateFillRead_WithMyAllocator) {
        my::list<int, my::allocator<int>> list;
        list.append(3);
        list.append(2);
        list.append(1);
        list.append(0);

        {
            boost::test_tools::output_test_stream output;
            cout_redirect guard(output.rdbuf());

        for(auto& element: list) {
            std::cout << element << std::endl;
        }
            BOOST_CHECK(output.is_equal("3\n2\n1\n0\n"));
        }
    }

    BOOST_AUTO_TEST_CASE(Destroy) {

        {
            boost::test_tools::output_test_stream output;
            cout_redirect guard(output.rdbuf());

            my::list<MyTestStruct, my::allocator<MyTestStruct>> list;
            list.append(MyTestStruct(3,true));
            list.append(MyTestStruct(2,true));
            list.append(MyTestStruct(1,true));
            list.append(MyTestStruct(0,true));

            BOOST_CHECK(output.is_equal("deleted 3\ndeleted 2\ndeleted 1\ndeleted 0\n"));
        }
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MemoryLeaksMyAlloc)
    BOOST_AUTO_TEST_CASE(std_list) {
        size_t alloc_before, alloc_after;
        alloc_before = my::alloc_counter;
        {
            auto gen = [i = 0]()mutable{return ++i;};
            std::list<int, my::allocator<int,10>> list;
            std::generate_n(std::back_inserter(list), 1000, gen);
        }
        alloc_after  = my::alloc_counter;
        BOOST_CHECK(alloc_before == alloc_after);
    }
    BOOST_AUTO_TEST_CASE(my_list) {
        size_t alloc_before, alloc_after;
        alloc_before = my::alloc_counter;
        {
            auto gen = [i = 0]()mutable{return int(++i);};
            my::list<int, my::allocator<int,10>> list;
            std::generate_n(std::back_inserter(list), 1000, gen);
        }
        alloc_after  = my::alloc_counter;
        BOOST_CHECK(alloc_before == alloc_after);
    }
    BOOST_AUTO_TEST_CASE(std_map) {
        size_t alloc_before, alloc_after;
        alloc_before = my::alloc_counter;
        {
            auto gen = [i = 0]()mutable{return std::make_pair(int(++i), int(i*i));};
            std::map<int, int, std::less<>,  my::allocator<int,10>> map;
            std::generate_n(std::inserter(map,std::begin(map)), 1000, gen);
        }
        alloc_after  = my::alloc_counter;
        BOOST_CHECK(alloc_before == alloc_after);
    }
BOOST_AUTO_TEST_SUITE_END()