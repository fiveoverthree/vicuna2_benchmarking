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
    float32_t output[N_SAMPLES];
    float32_t filter_state [N_TAPS + N_SAMPLES - 1];
    arm_fir_instance_f32 filter_S;
    float32_t output_initial[N_TAPS];

    public:
    /*
    * Required Functions for test framework
    */
    //
    Benchmark(){
        // filter initialization
        arm_fir_init_f32(&filter_S, N_TAPS, coeff, filter_state, N_SAMPLES);        // ignore the noisy outputs due to initial conditions
        for (uint32_t ptr = 0; ptr < (N_TAPS/N_SAMPLES); ptr++)
        {
            arm_fir_f32(&filter_S, input + (ptr * N_SAMPLES), output_initial + (ptr * N_SAMPLES), N_SAMPLES);
        }    
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        arm_fir_f32(&filter_S, (input + N_TAPS), output, N_SAMPLES);
        return 0; //cannot fail internally

    };

    //Validate Output
    int validate_benchmark()
    {
        // calculate SNR of test output vs matlab reference output
        float32_t snr;
        uint32_t fail_count = 0;
        snr = snr_f32(output_ref, output, N_SAMPLES);

        // check correctness (if reference and actual filter outputs matched)
        fail_count += (snr < SNR_REF_THLD);

        return fail_count;
    };
    //Cleanup any allocatations
    ~Benchmark(){
    };
};
#endif
