cmake_minimum_required(VERSION 3.22)
project(VKR)

set(CMAKE_CXX_STANDARD 14)

add_executable(VKR main.cpp binarymatrix.h binarymatrix.cpp ldpc.cpp ldpc.h inputvectorgenerator.cpp inputvectorgenerator.h)
