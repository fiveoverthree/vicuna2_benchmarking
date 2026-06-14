#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP 

#include <cstdint>
//BSP includes
//#include "uart.hpp"
//Includes for benchmark

extern "C"
{
    #include "snr.h"
    #include "data.h"
    #include "dsp/filtering_functions.h"
}

#include "dsp/filtering_functions.h"

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    float32_t out[TOTAL_SAMPLES];
    float32_t err[TOTAL_SAMPLES];
    float32_t filter_state[N_TAPS + N_SAMPLES - 1];

    arm_lms_instance_f32 filter_S;
    float32_t coeff[N_TAPS];
  

    public:
    /*
    * Required Functions for test framework
    */
    //
    Benchmark(){
        // filter initialization
        for (uint32_t i = 0; i<N_TAPS; i++)
        {
            coeff[i] = 0;
        }
        arm_lms_init_f32(&filter_S, N_TAPS, coeff, filter_state, mu, N_SAMPLES);
        // ignore the first N_INITIAL outputs (bad output based on zero initial state)
        for (uint32_t ptr = 0; ptr < (N_INITIAL/N_SAMPLES); ptr++)
        {
            arm_lms_f32(&filter_S, input + (ptr * N_SAMPLES), ref + (ptr * N_SAMPLES), out + (ptr * N_SAMPLES), err + (ptr * N_SAMPLES), N_SAMPLES);
        }
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        arm_lms_f32(&filter_S, input + N_INITIAL, ref + N_INITIAL, out + N_INITIAL, err + N_INITIAL, N_SAMPLES);
        return 0; //cannot fail internally

    };

    //Validate Output
    int validate_benchmark()
    {
        // calculate SNR of test output vs matlab reference output
        float32_t snr;
        uint32_t fail_count = 0;
        snr = snr_f32(output_ref + N_INITIAL, out + N_INITIAL, N_SAMPLES);

        // check correctness (if reference and actual filter outputs matched)
        fail_count += (snr < SNR_REF_THLD);

        return fail_count;
    };
    //Cleanup any allocatations
    ~Benchmark(){
    };
};
#endif
