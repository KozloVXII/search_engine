cmake_minimum_required(VERSION 3.6)

add_executable(${PROJECT_NAME}
                                main.cpp
                                ConverterJSON.cpp
                                InvertedIndex.cpp
                                SearchServer.cpp
                                ${PROJECT_INCLUDE_DIR}/ConverterJSON.h
                                ${PROJECT_INCLUDE_DIR}/SearchServer.h
                                ${PROJECT_INCLUDE_DIR}/InvertedIndex.h
                                ${PROJECT_FOLDER_NAME}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_TEST_DIR})

include(FetchContent)

FetchContent_Declare(JSON
        GIT_REPOSITORY https://github.com/nlohmann/json)
FetchContent_MakeAvailable(JSON)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS ${PROJECT_NAME}
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
target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})


