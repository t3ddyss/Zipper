cmake_minimum_required(VERSION 3.15)
project(Zipper)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zipper main.cpp Huffman.cpp Huffman.h LZ77.cpp LZ77.h)