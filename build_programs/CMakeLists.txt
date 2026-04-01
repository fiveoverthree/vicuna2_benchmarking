cmake_minimum_required(VERSION 3.10)
message(STATUS "CMake Version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

##########################
#
# Configuration variables:
# - RV_ARCH : select the processor architecture configuration for compilation
#
# FEATURES:
# - TRACE : enables .csv memory trace and .vcd signal trace output for unit tests.  Due to large file sizes, only trace of last test run will be kept.  Will be located in /build/Testing
#
#
##########################

#Set this on the command line to change configuration.  Currently Supported: rv32im, rv32imzve32x, rv32imf, rv32imfzve32x, rv32imfzve32f
set(RISCV_ARCH rv32im_zve32x CACHE STRING "Specify the configuration")
set(MEM_LATENCY 1 CACHE STRING "Memory latency")
set(MIN_VLEN 128 CACHE STRING "LLVM Autovectorizer parameter for minimum VLEN in bits")  #Does the same thing as _ZvlXXXb architecture string
set(MEM_W 32 CACHE STRING "Width of main memory data port in bits")

option(TRACE        "Enable trace outputs"                                         OFF)
option(PRINT        "Enable print statements"                                      OFF)


#####
# Default Paths to Subdirectories
#####

set(TOOLCHAIN_TOP ${CMAKE_CURRENT_SOURCE_DIR}/../toolchain)
set(VERILATOR_MODEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../processor_models/verilator)

#####
# Select RISCV_ARCH and RISCV_ABI and Autovectorization flags based on selected features
#####

# Import Vector config

include(${CMAKE_CURRENT_SOURCE_DIR}/../processor_models/vector_config.cmake)
message("Selected SCALAR_CORE = ${SCALAR_CORE}")
set(ENV{VPROC_PIPELINES} ${VPROC_PIPELINES})
message("Selected VPROC_PIPELINES = ${VPROC_PIPELINES}")
message("Selected RISCV_ARCH = ${RISCV_ARCH}")
message("Selected VREG_W = ${VREG_W}")
set(MEM_W ${VMEM_W})
message("Selected MEM_W = ${MEM_W}")


#These control benchmark configurations specific to the selected RISCV_ARCH
option(RISCV_F "Use the RISC-V F Extension" OFF)
option(RISCV_ZFH "Use the RISC-V ZFH Extension" OFF)
option(RISCV_ZVE32X "Use the RISC-V Embedded V Extension" OFF)
option(RISCV_ZVE32F "Use the RISC-V Embedded V Float Extension" OFF)
option(RISCV_ZVHF "Use the RISC-V V Half Float Extension" OFF)



set(AUTOVEC_FLAGS "")

if(${RISCV_ARCH} STREQUAL "rv32im") 
    set(RISCV_ABI ilp32)
    option(FH_OPS      "Use FP16 for intermediate operations (no conversion to float)"  OFF)
    
elseif(${RISCV_ARCH} STREQUAL "rv32imf")
    set(RISCV_ABI ilp32f)  
    set(RISCV_F ON)
    option(FH_OPS      "Use FP16 for intermediate operations (no conversion to float)"  OFF)
    
elseif(${RISCV_ARCH} STREQUAL "rv32imf_zfh")
    set(RISCV_ABI ilp32f)  
    set(RISCV_F ON)
    set(RISCV_ZFH ON)
    option(FH_OPS      "Use FP16 for intermediate operations (no conversion to float)"  ON)
    
elseif(${RISCV_ARCH} STREQUAL "rv32im_zve32x")
    set(RISCV_ABI ilp32) 
    set(RISCV_ZVE32X ON)
    set(AUTOVEC_FLAGS "")
    option(FH_OPS      "Use FP16 for intermediate operations (no conversion to float)"  OFF)
    
elseif(${RISCV_ARCH} STREQUAL "rv32imf_zve32f")
    set(RISCV_ABI ilp32f) 
    set(RISCV_ZVE32X ON)
    set(RISCV_ZVE32F ON)
    set(RISCV_F ON)
    #set(AUTOVEC_FLAGS "-fvectorize -fslp-vectorize -mllvm -scalable-vectorization=preferred -mllvm --riscv-v-vector-bits-min=${MIN_VLEN}")
    option(FH_OPS      "Use FP16 for intermediate operations (no conversion to float)"  OFF)
    
elseif(${RISCV_ARCH} STREQUAL "rv32imf_zfh_zve32f_zvfh")
    set(RISCV_ABI ilp32f) 
    set(RISCV_ZVE32X ON)
    set(RISCV_ZVE32F ON)
    set(RISCV_ZFH ON)
    set(RISCV_F ON)
    set(RISCV_ZVFH ON)
    #set(AUTOVEC_FLAGS "-fvectorize -fslp-vectorize -mllvm -scalable-vectorization=preferred -mllvm --riscv-v-vector-bits-min=${MIN_VLEN}")
    option(FH_OPS      "Use FP16 for intermediate operations (no conversion to float)"  ON) ##TODO: MOVE THIS TO BENCHMARKS TINYML
else()
    message(FATAL_ERROR "Unsupported RISCV_ARCH selected")
    
endif()

####
# Set build mode, Release by default
####
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(PRINT)
  add_definitions(-DPRINT_OUTPUTS) ##Todo, this should be moved
endif()

# Tell CMake about our processor
set(CMAKE_SYSTEM_PROCESSOR ${RISCV_ARCH})
# The Generic system name is used for bare-metal targets (without OS) in CMake
set(CMAKE_SYSTEM_NAME Generic)

# #####################
# # Setup the CMake Project
# ####################

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf) 
set(CMAKE_EXECUTABLE_SUFFIX ".elf") #For some reason setting this works for ASM files but SUFFIX_ASM doesnt

set(CMAKE_TOOLCHAIN_FILE "/home/parker/Desktop/vicuna2_generic_benchmarking/vicuna2_benchmarking/benchmark_sources/MLPerfTiny/CMake/gcc_toolchain.cmake")
project(Vicuna2_Benchmarking_Environment LANGUAGES C CXX ASM DESCRIPTION "Vicuna2 Benchmarking Environment")
include(CTest)

########
#Add Program Subdirectories
########

add_subdirectory(../benchmark_sources benchmark_sources)
#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../benchmark_sources/MLPerfTiny/CMake/gcc_toolchain.cmake")




