cmake_minimum_required(VERSION 3.8)
project(sample LANGUAGES CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/xcspp)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

add_executable(sample src/main.cpp)
target_compile_features(sample PRIVATE cxx_std_17)
if (MSVC)
    target_compile_options(sample PRIVATE /W4)
else()
    target_compile_options(sample PRIVATE -O2 -Wall)
endif()
target_link_libraries(sample xcspp)
