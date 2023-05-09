cmake_minimum_required(VERSION 3.25)
project(ProgramFirst_Client)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -s -O3 -I C:/Users/Shiro/Desktop/asio-1.10.6/include")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -static")
link_libraries(ws2_32 wsock32)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} program_first.cpp)

target_link_libraries(ProgramFirst_Client PRIVATE nlohmann_json::nlohmann_json)
