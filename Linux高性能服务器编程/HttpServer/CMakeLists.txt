cmake_minimum_required(VERSION 3.10)
project(TinyHttpServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(httpserver src/main.cpp src/http_server.cpp)

target_link_libraries(httpserver pthread)