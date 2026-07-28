cmake_minimum_required(VERSION 3.20)

project(ChatServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(ChatServer
    src/main.cpp
    src/Server.cpp
)


target_include_directories(ChatServer PRIVATE
    include
)
