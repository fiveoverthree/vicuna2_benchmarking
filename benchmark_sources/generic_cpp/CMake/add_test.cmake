macro(add_benchmark TEST TEST_NUM) 
    
    set(TEST_NAME ${TEST}_${TEST_NUM})
    add_executable(${TEST_NAME})

    target_include_directories(${TEST_NAME} PUBLIC
        ${VICUNA_BSP_TOP}/../                   # Becomes Framework top
        ${VICUNA_BSP_TOP}/lib/
        ${PROGRAMS_TOP}/${TEST}
        ${PROGRAMS_TOP}/${TEST}/test_data
    )

    target_sources(${TEST_NAME} PUBLIC
        ${VICUNA_BSP_TOP}/../main.cpp           # Becomes Framework top
        ${PROGRAMS_TOP}/${TEST}/run_test.hpp
        ${PROGRAMS_TOP}/${TEST}/run_test.cpp
        ${PROGRAMS_TOP}/${TEST}/test_data.hpp
        ${PROGRAMS_TOP}/${TEST}/test_data/test_data_${TEST_NUM}.cpp
        ${PROGRAMS_TOP}/${TEST}/${TEST}.hpp
        ${PROGRAMS_TOP}/${TEST}/${TEST}.cpp
        #${VICUNA_BSP_TOP}/lib/vicuna_crt.h
    )

    #Set Linker
    target_link_options(${TEST_NAME} PRIVATE "-nostartfiles")

    target_link_options(${TEST_NAME} PRIVATE "-T${VICUNA_BSP_TOP}/lld_link.ld")


    #Link BSP
    target_link_libraries(${TEST_NAME} PUBLIC bsp_Vicuna UART_Vicuna)

    add_custom_command(TARGET ${TEST_NAME}
                       POST_BUILD
                       COMMAND ${CMAKE_OBJCOPY} -O binary ${TEST_NAME}.elf ${TEST_NAME}.bin
                       COMMAND srec_cat ${TEST_NAME}.bin -binary -offset 0x0000 -byte-swap 4 -o ${TEST_NAME}.vmem -vmem
                       COMMAND rm -f prog_${TEST_NAME}.txt
                       COMMAND echo -n "${TEST_BUILD_DIR}/${TEST_NAME}.vmem" > prog_${TEST_NAME}.txt
                       COMMAND ${CMAKE_OBJDUMP} -D ${TEST_NAME}.elf > ${TEST_NAME}_dump.txt
                       )
    
    #VERY DANGEROUS TO USE TRACE
    if(TRACE)
        set(VCD_TRACE_ARGS "${TEST_BUILD_DIR}/test_${TEST_NAME}_sig.vcd")

    else()
        set(VCD_TRACE_ARGS "")
    endif()
                       
	              

    #Add Test
    add_test(NAME ${TEST_NAME} 
             COMMAND ./${VERILATOR_MODEL_DIR}/build/verilated_model ${TEST_BUILD_DIR}/prog_${TEST_NAME}.txt ${MEM_W} 4194304 ${MEM_LATENCY} 1 ${TEST_NAME} ${VREG_W} 0 ${VCD_TRACE_ARGS}#TODO: PASS ALL THESE ARGUMENTS IN FROM USER
             WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../..)
             
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 120) #TODO: Find a reasonable timeout for these tests

    message(STATUS "Successfully added ${TEST_NAME}")


endmacro()
