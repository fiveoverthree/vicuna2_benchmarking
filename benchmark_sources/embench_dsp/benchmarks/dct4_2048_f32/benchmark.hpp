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
    #include "arm_common_tables.h"
}

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    float32_t dct4_state[DCT4_SIZE*2];
    float32_t * output;
    arm_rfft_fast_instance_f32  rfft_S;
    arm_dct4_instance_f32  dct4_S;
  

    public:
    /*
    * Required Functions for test framework
    */
    //
    Benchmark(){
        //initialization
        output = inout; //output pointer is the statically declared input pointer
        arm_rfft_fast_init_2048_f32(&rfft_S);
        dct4_S = {DCT4_SIZE, DCT4_SIZE/2, 
                                   0.03125f,          // normalizing factor is sqrt(2/DCT4_SIZE)
                                   Weights_2048,      // twiddle coefficients
                                   cos_factors_2048,  // cosine factors
                                   &rfft_S,           // real FFT instance (pre-initialized)
                                   &(rfft_S.Sint),    // complex FFT instance (initialized by RFFT)
                                   };
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        arm_dct4_f32(&dct4_S, &dct4_state[0], &inout[0]);
        return 0; //cannot fail internally

    };

    //Validate Output
    int validate_benchmark()
    {
        // calculate SNR of test output vs matlab reference output
        float32_t snr;
        uint32_t fail_count = 0;
        snr = snr_f32(output_ref, inout, DCT4_SIZE);

        // check correctness (if reference and actual filter outputs matched)
        fail_count += (snr < SNR_REF_THLD);

        return fail_count;
    };
    //Cleanup any allocatations
    ~Benchmark(){
    };
};
#endif
