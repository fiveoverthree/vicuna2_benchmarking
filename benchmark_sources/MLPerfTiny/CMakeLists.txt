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
    execute_process(COMMAND ./configure --prefix=${TOOLCHAIN_TOP}/GCC/multilib --with-arch=rv32im_zve32x --with-abi=ilp32 --enable-multilib   #Multilib build should include all valid configs for ANY benchmark for --with-arch
                    WORKING_DIRECTORY ${TOOLCHAIN_TOP}/riscv-gnu-toolchain/)
    execute_process(COMMAND make -j8 #Build crashes with -j${nproc}?
                    WORKING_DIRECTORY ${TOOLCHAIN_TOP}/riscv-gnu-toolchain/)

endif()

# Add test addition macros
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/add_test.cmake)


# Add tflm directory to build library
add_subdirectory(tflm tflm)

###########
# Add tests for the benchmark suite
###########

#Needed to run TensorFlow code with RISC-V toolchain.  These flags need to be defined at this level and at tflm library level TODO: investigate
add_definitions(-DTF_LITE_USE_GLOBAL_MAX)
add_definitions(-DTF_LITE_USE_GLOBAL_MIN)
add_definitions(-DTF_LITE_STATIC_MEMORY)
add_definitions(-DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS)
if(NOT RISCV_F)
    add_definitions(-DTFLITE_EMULATE_FLOAT)                         #Might have issues with this for F extensions
endif()
add_definitions(-fno-rtti)          #Needed for GCC to resolve virtual delete function scope issue
add_definitions(-fno-exceptions)    #Needed for GCC to resolve virtual delete function scope issue

#Set location for build binaries
set(BINARY_DIR ${BUILD_DIR}/benchmark_sources/MLPerfTiny/)

add_Benchmark(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR})
#add_Benchmark_Gem5(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR} gem5_caches)
#add_Benchmark_Hybrid(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR} hybrid_caches)
#add_Benchmark_Hybrid(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR} hybrid_only)
add_Benchmark_Spike(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR})
