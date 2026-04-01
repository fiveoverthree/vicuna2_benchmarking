## This benchmark uses Multilib GCC.  Make sure it is built and present in the toolchain folder

## Does GCC Multilib Build Exist? If not build it
if(NOT EXISTS "${TOOLCHAIN_TOP}/GCC/multilib")
    message("GCC Multilib not found. Building")
    #Make GCC directory if it doesnt exist
    if(NOT EXISTS "${TOOLCHAIN_TOP}/GCC")
        execute_process(COMMAND mkdir GCC
                        WORKING_DIRECTORY ${TOOLCHAIN_TOP})
    endif()
    #riscv-gnu-toolchain submodule is not downloaded with --recursive flag.  Need to init if not done already
    if(NOT EXISTS "${TOOLCHAIN_TOP}/riscv-gnu-toolchain/gcc")
        execute_process(COMMAND git submodule update --init toolchain/riscv-gnu-toolchain/
                        WORKING_DIRECTORY ${TOOLCHAIN_TOP})
    endif()
    execute_process(COMMAND make clean
                    WORKING_DIRECTORY ${TOOLCHAIN_TOP}/riscv-gnu-toolchain/)
    execute_process(COMMAND ./configure --prefix=${TOOLCHAIN_TOP}/GCC/multilib --with-arch=rv32im_zve32x --with-abi=ilp32 --enable-multilib   #Multilib build should include all valid configs for ANY benchmark
                    WORKING_DIRECTORY ${TOOLCHAIN_TOP}/riscv-gnu-toolchain/)
    execute_process(COMMAND make -j8 #Build crashes with -j${nproc}?
                    WORKING_DIRECTORY ${TOOLCHAIN_TOP}/riscv-gnu-toolchain/)

endif()

# Add Toolchain File

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMake/gcc_toolchain.cmake")
# Add test addition macros
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/add_test.cmake)


# Add tflm directory to build library
add_subdirectory(tflm tflm)

###########
# Add tests for the benchmark suite
###########

add_Benchmark(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BUILD_DIR}/benchmark_sources/MLPerfTiny/)
#add_Benchmark_Spike(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR} ${BUILD_DIR}/benchmark_sources/Int8/toycar)
