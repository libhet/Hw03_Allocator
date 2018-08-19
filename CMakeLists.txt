cmake_minimum_required(VERSION 3.9)
project(allocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(allocator
        src/main.cpp
        src/my_allocator.h
        src/my_newdelete.h
        src/my_newdelete.cpp
        src/my_list.h
        )

set_target_properties(allocator PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS -Wpedantic -Wall -Wextra
        )

add_subdirectory(tests)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT shadow.balz@mail.ru)

include(CPack)

enable_testing()