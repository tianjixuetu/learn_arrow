cmake_minimum_required(VERSION 3.16)

project(MyExample)

find_package(Arrow REQUIRED)


add_executable(my_example my_example.cc)
target_link_libraries(my_example PRIVATE Arrow::arrow_shared )