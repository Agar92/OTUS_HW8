cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
project(bayan VERSION ${PROJECT_VERSION})
configure_file(version.h.in version.h)

find_package(Boost COMPONENTS program_options unit_test_framework filesystem regex REQUIRED)

add_executable(bayan main.cpp)
add_library(lib crc32_hash.cpp md5_hash.cpp settings.cpp file_comparison.cpp searcher.cpp)
add_executable(test_bayan_options test_options.cpp)
add_executable(test_bayan_search  test_search.cpp)

set_target_properties(bayan lib test_bayan_options test_bayan_search PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
#message("CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(bayan PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})
target_include_directories(lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(test_bayan_options test_bayan_search PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(lib PRIVATE
    ${Boost_LIBRARIES}
)

target_link_libraries(bayan PRIVATE
    ${Boost_LIBRARIES}
    lib
)
target_link_libraries(test_bayan_options
    ${Boost_LIBRARIES}
    lib
)

target_link_libraries(test_bayan_search
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
    target_compile_options(test PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror -g
    )
    target_compile_options(lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_bayan_options PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_bayan_search PRIVATE
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

enable_testing()

add_test(test_bayan_options test_bayan_options)
add_test(test_bayan_search  test_bayan_search)
