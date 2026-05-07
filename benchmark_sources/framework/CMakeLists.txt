add_subdirectory(vicuna2_bsp vicuna2_bsp) 
set(VICUNA_BSP_TOP ${CMAKE_CURRENT_SOURCE_DIR}/vicuna2_bsp/ PARENT_SCOPE)
set(FRAMEWORK_TOP ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)

#Add header library for Verilator only simulation specific implementations
add_library(sim_Verilator INTERFACE)

target_include_directories(sim_Verilator INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/verilator
)

#Add library for Verilator only simulation specific implementations