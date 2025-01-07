cmake_minimum_required(VERSION 3.30)
project(DiplomaProject)

set(CMAKE_CXX_STANDARD 17)

#set(JSON_BuildTests OFF CACHE INTERNAL "")
#
#add_subdirectory(nlohmann_json)
#target_link_libraries(DiplomaProject PRIVATE nlohmann_json::nlohmann_json)
add_executable(DiplomaProject   main.cpp
                                ConverterJSON.h
                                ConverterJSON.cpp
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

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        # Specify the commit you depend on and update it regularly.
##        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
#        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
## Now simply link against gtest or gtest_main as needed. Eg
##add_executable(DiplomaProject example.cpp)
#target_link_libraries(DiplomaProject PRIVATE gtest_main)
#add_test(NAME example_test COMMAND example)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(DiplomaProject PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(DiplomaProject)