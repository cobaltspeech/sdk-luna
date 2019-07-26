# Copyright (2019) Cobalt Speech and Language, Inc.

cmake_minimum_required(VERSION 3.5.0)
project(luna_client)

include(CobaltGRPC.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate the grpc files from our proto files
run_protoc(RESULT_FILES LUNA_GRPCFILES
    PROTOS ../luna.proto
    OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Setup the library
add_library(luna_client
    ${LUNA_GRPCFILES}
    luna_client.cpp
    luna_client.h
    luna_exception.cpp
    luna_exception.h
    luna_synthesizer_stream.cpp
    luna_synthesizer_stream.h
    luna_voice.cpp
    luna_voice.h)

# Setup the linking and include directories for the library.
# We give it PUBLIC visibility so other targets can use the
# include directories and libraries.
target_include_directories(luna_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(luna_client PUBLIC
    grpc grpc++ libprotobuf)

