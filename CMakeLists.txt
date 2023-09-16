cmake_minimum_required(VERSION 3.11...3.16)

set(PYTHON_EXECUTABLE "/usr/local/bin/python")


project(MyProject VERSION 1.0
                 DESCRIPTION "MyProject with pybind11"
                 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

add_executable(MyProject main.cpp)

target_link_libraries(MyProject PRIVATE pybind11::embed)
