cmake_minimum_required(VERSION 3.10)
#####################################
#
# This file contains all sources for SystemVerilog Models
#
#####################################
#Path to CV32E40X sources               
set(DESIGN_RTL_DIR_CV32E40X ${CMAKE_CURRENT_SOURCE_DIR}/cv32e40x/rtl)
#Sources Needed for CV32E40X
set(CV32E40X_SOURCE ${DESIGN_RTL_DIR_CV32E40X}/include/cv32e40x_pkg.sv                  
                    ${DESIGN_RTL_DIR_CV32E40X}/if_c_obi.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/if_xif.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/../bhv/cv32e40x_sim_clock_gate.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_core.sv 
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_a_decoder.sv 
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_alignment_buffer.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_alu.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_alu_b_cpop.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_b_decoder.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_clic_int_controller.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_compressed_decoder.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_controller.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_controller_bypass.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_controller_fsm.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_csr.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_cs_registers.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_data_obi_interface.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_decoder.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_div.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_ex_stage.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_ff_one.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_i_decoder.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_id_stage.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_if_stage.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_instr_obi_interface.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_int_controller.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_load_store_unit.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_lsu_response_filter.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_m_decoder.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_mpu.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_mult.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_pc_target.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_pma.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_popcnt.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_prefetcher.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_prefetch_unit.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_register_file.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_register_file_wrapper.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_sequencer.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_sleep_unit.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_wb_stage.sv
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_write_buffer.sv
                    PARENT_SCOPE)
               
#CV32E40X Include Dirs
set(CV32E40X_INCLUDE ${DESIGN_RTL_DIR_CV32E40X}                                         
                    ${DESIGN_RTL_DIR_CV32E40X}/include
                    ${DESIGN_RTL_DIR_CV32E40X}/../bhv
                    PARENT_SCOPE)

#####################################
#Path to FPU_SS sources
set(DESIGN_RTL_DIR_FPU_SS ${CMAKE_CURRENT_SOURCE_DIR}/fpu_ss/src)
#FPU_SS Sources                   
set(FPU_SS_SOURCE   ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_pkg.sv                              
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_prd_f_pkg.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_prd_f_zfh_pkg.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_prd_zfinx_pkg.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_instr_pkg.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_compressed_predecoder.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_controller.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_csr.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_decoder.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_predecoder.sv
                    ${DESIGN_RTL_DIR_FPU_SS}/fpu_ss_regfile.sv 
                    PARENT_SCOPE)

#FPU_SS Include Dirs
set(FPU_SS_INCLUDE  ${DESIGN_RTL_DIR_FPU_SS} PARENT_SCOPE)
#####################################
#Path to CV32A60X sources
#Just include sources to instantiate a core, not full soc system.    TODO:  REMOVE UNECESSARY SOURCES           
set(CVA6_REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cv32a60x)
set(HPDCACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cv32a60x/core/cache_subsystem/hpdcache)
#Sources Needed for CV32A60X. In the order from Flist.cva6
set(CV32A60X_SOURCE ${CVA6_REPO_DIR}/vendor/pulp-platform/fpga-support/rtl/SyncDpRam.sv #FPGA Memory
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/fpga-support/rtl/AsyncDpRam.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/fpga-support/rtl/AsyncThreePortRam.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/fpga-support/rtl/SyncThreePortRam.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/fpga-support/rtl/SyncDpRam_ind_r_w.sv
                    
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_pkg.sv        #FPNEW Floating point unit
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_cast_multi.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_classifier.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_divsqrt_multi.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_fma_multi.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_fma.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_noncomp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_opgroup_block.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_opgroup_fmt_slice.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_opgroup_multifmt_slice.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_rounding.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpnew_top.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/defs_div_sqrt_mvp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/control_mvp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/div_sqrt_top_mvp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/iteration_div_sqrt_mvp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/norm_div_sqrt_mvp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/nrbd_nrsc_mvp.sv
                    ${CVA6_REPO_DIR}/core/cvfpu/src/fpu_div_sqrt_mvp/hdl/preprocess_mvp.sv

                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/cf_math_pkg.sv #packages
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_pkg.sv
                    ${CVA6_REPO_DIR}/core/include/config_pkg.sv
                    ${CVA6_REPO_DIR}/core/include/cv32a60x_config_pkg.sv
                    ${CVA6_REPO_DIR}/core/include/riscv_pkg.sv

                    ${DESIGN_RTL_DIR_CV32E40X}/include/cv32e40x_pkg.sv  #VPROC VDIV has a dependency on this package  TODO: REMOVE IT
                    ${DESIGN_RTL_DIR_CV32E40X}/cv32e40x_ff_one.sv
                    
                    ${CVA6_REPO_DIR}/core/include/ariane_pkg.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/axi/src/axi_pkg.sv

                    ${CVA6_REPO_DIR}/core/include/wt_cache_pkg.sv
                    ${CVA6_REPO_DIR}/core/include/std_cache_pkg.sv
                    ${CVA6_REPO_DIR}/core/include/instr_tracer_pkg.sv
                    ${CVA6_REPO_DIR}/core/include/build_config_pkg.sv

                    ${CVA6_REPO_DIR}/core/cvxif_compressed_if_driver.sv             #CVXIF
                    ${CVA6_REPO_DIR}/core/cvxif_issue_register_commit_if_driver.sv
                    ${CVA6_REPO_DIR}/core/cvxif_example/include/cvxif_instr_pkg.sv
                    ${CVA6_REPO_DIR}/core/cvxif_fu.sv
                    ${CVA6_REPO_DIR}/core/cvxif_example/cvxif_example_coprocessor.sv
                    ${CVA6_REPO_DIR}/core/cvxif_example/instr_decoder.sv
                    ${CVA6_REPO_DIR}/core/cvxif_example/compressed_instr_decoder.sv
                    ${CVA6_REPO_DIR}/core/cvxif_example/copro_alu.sv

                    #${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/fifo_v3.sv   #Common Cells Included in Vicuna
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/lfsr.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/lfsr_8bit.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/stream_arbiter.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/stream_arbiter_flushable.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/stream_mux.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/stream_demux.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/lzc.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/rr_arb_tree.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/shift_reg.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/popcount.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/exp_backoff.sv

                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/counter.sv   #Common cells for example co-processor
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/delta_counter.sv

                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_atop_resolver.sv  #OBI
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_demux.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_err_sbr.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_intf.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_mux.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_sram_shim.sv
                    ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/src/obi_xbar.sv

                    ${CVA6_REPO_DIR}/core/cva6.sv               #Top level sources
                    ${CVA6_REPO_DIR}/core/cva6_pipeline.sv
                    ${CVA6_REPO_DIR}/core/cva6_rvfi_probes.sv
                    ${CVA6_REPO_DIR}/core/alu.sv
                    
                    ${CVA6_REPO_DIR}/core/fpu_wrap.sv           #depends on fpu-pkg
                    ${CVA6_REPO_DIR}/core/branch_unit.sv
                    ${CVA6_REPO_DIR}/core/compressed_decoder.sv
                    ${CVA6_REPO_DIR}/core/macro_decoder.sv
                    ${CVA6_REPO_DIR}/core/controller.sv
                    ${CVA6_REPO_DIR}/core/zcmt_decoder.sv
                    ${CVA6_REPO_DIR}/core/csr_buffer.sv
                    ${CVA6_REPO_DIR}/core/csr_regfile.sv
                    ${CVA6_REPO_DIR}/core/decoder.sv
                    ${CVA6_REPO_DIR}/core/ex_stage.sv
                    ${CVA6_REPO_DIR}/core/instr_realign.sv
                    ${CVA6_REPO_DIR}/core/id_stage.sv
                    ${CVA6_REPO_DIR}/core/issue_read_operands.sv
                    ${CVA6_REPO_DIR}/core/issue_stage.sv
                    ${CVA6_REPO_DIR}/core/load_unit.sv
                    ${CVA6_REPO_DIR}/core/load_store_unit.sv
                    ${CVA6_REPO_DIR}/core/lsu_bypass.sv
                    ${CVA6_REPO_DIR}/core/mult.sv
                    ${CVA6_REPO_DIR}/core/multiplier.sv
                    ${CVA6_REPO_DIR}/core/serdiv.sv
                    ${CVA6_REPO_DIR}/core/perf_counters.sv
                    ${CVA6_REPO_DIR}/core/ariane_regfile_ff.sv
                    ${CVA6_REPO_DIR}/core/ariane_regfile_fpga.sv
                    
                    ${CVA6_REPO_DIR}/core/scoreboard.sv
                    ${CVA6_REPO_DIR}/core/store_buffer.sv
                    ${CVA6_REPO_DIR}/core/amo_buffer.sv
                    ${CVA6_REPO_DIR}/core/store_unit.sv
                    ${CVA6_REPO_DIR}/core/commit_stage.sv
                    ${CVA6_REPO_DIR}/core/axi_shim.sv
                    ${CVA6_REPO_DIR}/core/cva6_accel_first_pass_decoder_stub.sv
                    ${CVA6_REPO_DIR}/core/acc_dispatcher.sv
                    ${CVA6_REPO_DIR}/core/cva6_fifo_v3.sv

                    
                    ${CVA6_REPO_DIR}/core/frontend/btb.sv
                    ${CVA6_REPO_DIR}/core/frontend/bht.sv
                    ${CVA6_REPO_DIR}/core/frontend/ras.sv
                    ${CVA6_REPO_DIR}/core/frontend/instr_scan.sv
                    ${CVA6_REPO_DIR}/core/frontend/instr_queue.sv
                    ${CVA6_REPO_DIR}/core/frontend/frontend.sv
                    
                    ${CVA6_REPO_DIR}/core/pmp/src/pmp.sv    #Physical Memory Protection NOTE: pmp.sv modified for DSIM (unchanged for other simulators)
                    ${CVA6_REPO_DIR}/core/pmp/src/pmp_entry.sv
                    ${CVA6_REPO_DIR}/core/pmp/src/pmp_data_if.sv
                    PARENT_SCOPE)
    

set(CV32A60X_INCLUDE ${CVA6_REPO_DIR}/core/include/
                     ${CVA6_REPO_DIR}/vendor/pulp-platform/obi/include/
                     #${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/include/ #included in Vicuna repo
                     ${CVA6_REPO_DIR}/vendor/pulp-platform/common_cells/src/
                     ${CVA6_REPO_DIR}/vendor/pulp-platform/axi/include/
                     ${CVA6_REPO_DIR}/common/local/util/
                     #${CVA6_REPO_DIR}/core/cvfpu/src/common_cells/include/ #Include path for cvfpu #Might have issues with other copy of cvfpu in vicuna2 core
                     ${HPDCACHE_DIR}/rtl/include
                     PARENT_SCOPE)





#################
#Path to Vicuna Sources  Include the CMakeList file from the repo
set(VICUNA_TOP_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/vicuna2_core PARENT_SCOPE)  
add_subdirectory(vicuna2_core vicuna) # Include vicuna and cvfpu with VICUNA_SRCS, VICUNA_INCS

#Pass Vicuna files up to parent CMAKEList
set(VICUNA_SRCS ${VICUNA_SRCS} PARENT_SCOPE)
set(VICUNA_INCS ${VICUNA_INCS} PARENT_SCOPE)

