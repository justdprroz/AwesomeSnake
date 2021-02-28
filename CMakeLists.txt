cmake_minimum_required(VERSION 3.17)
project("Awesome Snake")

set(CMAKE_CXX_STANDARD 17)

include_directories("include")
link_directories("lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

add_executable(client source/client.cpp source/snake/snakebase.cpp source/snake/snakeclient.cpp)
add_executable(server source/server.cpp source/snake/snakebase.cpp source/snake/snakeserver.cpp)

target_link_libraries(client -lsfml-graphics -lsfml-window -lsfml-system -pthread)
target_link_libraries(server -pthread)