cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(bayan VERSION ${PROJECT_VERSION})
configure_file(version.h.in version.h)

find_package(Boost COMPONENTS program_options unit_test_framework filesystem regex REQUIRED)

add_executable(bayan main.cpp)
add_library(lib crc32_hash.cpp md5_hash.cpp settings.cpp file_comparison.cpp searcher.cpp)

set_target_properties(bayan lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
#message("CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(bayan PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
target_include_directories(lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lib PRIVATE
    ${Boost_LIBRARIES}
)

target_link_libraries(bayan PRIVATE
    ${Boost_LIBRARIES}
    lib
)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(lib PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror -g
    )
    target_compile_options(lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT agar10@yandex.ru)

include(CPack)
