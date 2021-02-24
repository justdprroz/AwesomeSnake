cmake_minimum_required(VERSION 3.17)
project("Awesome Snake")

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories("include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)

# add_executable(snakeclient source/snakeclient.cpp source/snake/snake.cpp)
# add_executable(snakeserver source/snakeserver.cpp source/snake/snake.cpp)

add_executable(client source/client.cpp source/snake/snakebase.cpp source/snake/snakeclient.cpp)
add_executable(server source/server.cpp source/snake/snakebase.cpp source/snake/snakeserver.cpp)

# target_link_libraries(snakeclient sfml-graphics -pthread)
# target_link_libraries(snakeserver sfml-graphics -pthread)

target_link_libraries(client sfml-graphics -pthread)
target_link_libraries(server -pthread)