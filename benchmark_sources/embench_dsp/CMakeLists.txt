###########
# Add test addition macros
###########
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/add_test.cmake)


###########
# Create Lib - benchmark suite has its own reduced version of CMSIS-DSP
###########
set(EMBENCH_DSP_TOP ${CMAKE_CURRENT_SOURCE_DIR}/)
set(DSP_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/embench-dsp/lib/)

add_library(dsp-lib STATIC)

set_target_properties(dsp-lib PROPERTIES LINKER_LANGUAGE C)

target_sources(dsp-lib PUBLIC
    ${DSP_LIB_PATH}/src/BasicMathFunctions/arm_mult_f32.c
    ${DSP_LIB_PATH}/src/BasicMathFunctions/arm_scale_f32.c 
    ${DSP_LIB_PATH}/src/CommonTables/arm_common_tables.c
    ${DSP_LIB_PATH}/src/CommonTables/arm_const_structs.c
    ${DSP_LIB_PATH}/src/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c  
    ${DSP_LIB_PATH}/src/FilteringFunctions/arm_biquad_cascade_df2T_f32.c
    ${DSP_LIB_PATH}/src/FilteringFunctions/arm_biquad_cascade_df2T_init_f32.c
    ${DSP_LIB_PATH}/src/FilteringFunctions/arm_fir_f32.c
    ${DSP_LIB_PATH}/src/FilteringFunctions/arm_fir_init_f32.c
    ${DSP_LIB_PATH}/src/FilteringFunctions/arm_lms_f32.c
    ${DSP_LIB_PATH}/src/FilteringFunctions/arm_lms_init_f32.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_bitreversal2.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_cfft_f32.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_cfft_init_f32.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_cfft_radix8_f32.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_dct4_f32.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_rfft_fast_f32.c
    ${DSP_LIB_PATH}/src/TransformFunctions/arm_rfft_fast_init_f32.c
    ${EMBENCH_DSP_TOP}/embench-dsp/support/snr.c
)

target_include_directories(dsp-lib PUBLIC
   ${DSP_LIB_PATH}/include
   ${DSP_LIB_PATH}/include/dsp
   ${DSP_LIB_PATH}/src/BasicMathFunctions
   ${DSP_LIB_PATH}/src/CommonTables
   ${DSP_LIB_PATH}/src/ComplexMathFunctions
   ${DSP_LIB_PATH}/src/FilteringFunctions
   ${DSP_LIB_PATH}/src/TransformFunctions
   ${EMBENCH_DSP_TOP}/embench-dsp/
)

###########
# Add tests for the benchmark suite
###########

#Needed to run TensorFlow code with RISC-V toolchain.  These flags need to be defined at this level and at tflm library level TODO: investigate

#Set location for build binaries
set(BINARY_DIR ${BUILD_DIR}/benchmark_sources/embench_dsp/)

file(GLOB folders "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*" )
foreach(item ${folders})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/" "" bm ${item})
    add_Benchmark_Verilator(${bm})
    #add_Benchmark_Gem5(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR} gem5_caches)
    #add_Benchmark_Hybrid(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR} hybrid_caches)
    #add_Benchmark_Hybrid(toycar_int8 ${CMAKE_CURRENT_SOURCE_DIR}/Int8/toycar/ ${BINARY_DIR} hybrid_only)
    add_Benchmark_Spike(${bm})
endforeach()

