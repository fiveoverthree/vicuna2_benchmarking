########
# Add framework directory including bsps, support files, and simulator specific functions.
add_subdirectory(framework framework)
########

#Select benchmark suite build directory.  Since each benchmark suite uses a different compiler setup, only one can be active at a time.
if(NOT BENCHMARK_SUITE)
    message(FATAL_ERROR "No Benchmark Suite selected.  Select a valid suite with -DBENCHMARK_SUITE=(name)")
endif()

if(${BENCHMARK_SUITE} STREQUAL "generic_cpp")
    add_subdirectory(generic_cpp generic_cpp)
elseif(${BENCHMARK_SUITE} STREQUAL "MLPerfTiny")
    add_subdirectory(MLPerfTiny MLPerfTiny)
elseif(${BENCHMARK_SUITE} STREQUAL "embench_dsp")
    add_subdirectory(embench_dsp embench_dsp)
else()
    message(FATAL_ERROR "Unsupported Benchmark Suite selected.  Select a valid suite with -DBENCHMARK_SUITE=(name)")
endif()



