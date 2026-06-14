#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP 

#include <cstdint>
//BSP includes
//#include "uart.hpp"
//Includes for benchmark

extern "C"
{
    #include "test_main.h"
    #include "snr.h"
    #include "data.h"
    #include "dsp/filtering_functions.h"
}

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    float32_t output[FFT_SIZE];
    arm_rfft_fast_instance_f32 arm_rfft_fast_S;
  

    public:
    /*
    * Required Functions for test framework
    */
    //
    Benchmark(){
        // filter initialization
        arm_rfft_fast_init_2048_f32(&arm_rfft_fast_S);
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        arm_rfft_fast_f32(&arm_rfft_fast_S, input, output, IFFT_FLAG);
        return 0; //cannot fail internally

    };

    //Validate Output
    int validate_benchmark()
    {
        // calculate SNR of test output vs matlab reference output
        float32_t snr;
        uint32_t fail_count = 0;
        snr = snr_f32(output_ref, output, FFT_SIZE);

        // check correctness (if reference and actual filter outputs matched)
        fail_count += (snr < SNR_REF_THLD);

        return fail_count;
    };
    //Cleanup any allocatations
    ~Benchmark(){
    };
};
#endif
