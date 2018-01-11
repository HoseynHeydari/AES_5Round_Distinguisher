cmake_minimum_required(VERSION 3.8)
project(AES_5Round_Distinguisher)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c main.h)
add_executable(AES_5Round_Distinguisher ${SOURCE_FILES})