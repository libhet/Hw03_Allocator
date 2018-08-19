#if(BUILD_TESTING)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(
        test_allocator
        test_allocator.cpp
        ../src/my_newdelete.cpp
)

set_target_properties(test_allocator PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS}
        )

target_link_libraries(test_allocator ${Boost_LIBRARIES})

add_test(TEST_ALLOCATOR test_allocator)
#endif()