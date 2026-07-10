cmake_minimum_required(VERSION 3.10)

project(etiss_crt0 C CXX ASM)

add_library(${PROJECT_NAME} STATIC crt0.S fixes.c trap_handler.c)

target_link_options(${PROJECT_NAME} PRIVATE "-Tetiss.ld")