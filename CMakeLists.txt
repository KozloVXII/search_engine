cmake_minimum_required(VERSION 3.6)
project(DiplomaProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(DiplomaProject   main.cpp
                                ConverterJSON.h
                                ConverterJSON.cpp
                                InvertedIndex.cpp
                                InvertedIndex.h
                                SearchServer.h
                                SearchServer.cpp
)

include(FetchContent)

FetchContent_Declare(JSON
        GIT_REPOSITORY https://github.com/nlohmann/json)
FetchContent_MakeAvailable(JSON)
target_link_libraries(DiplomaProject PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS DiplomaProject
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads ON)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#enable_testing()
target_link_libraries(DiplomaProject PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(DiplomaProject)
