#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP 

#include <cstdint>
//BSP includes
#include "uart.hpp"
//Includes for benchmark
#include "test_data.hpp"
#include "mul.hpp"

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    test_metadata meta;
    uint32_t vecLen;

    int32_t *output;

    int16_t* vec_a;
    int16_t* vec_b;

    void report_diff(int32_t* output, int32_t* reference, uint32_t vecLen)
    {
        uart_printf("Your Result:\n\n");
        for(uint32_t i=0; i<vecLen; i++)
        {
            uart_printf("%d ",output[i]);
        }
        uart_printf("\n");
        uart_printf("Reference Result:\n\n");
        for(uint32_t i=0; i<vecLen; i++)
        {
                uart_printf("%d ",reference[i]);
        }
        uart_printf("\n");
    }

    void report_metadata(uint32_t vecLen)
    {
        uart_printf("Testcase:\n\n");
        uart_printf("Vector Multiplication - Vectors %d elements long:\n\n", vecLen);

    }

    public:
    /*
    * Required Functions for test framework
    */
    //override constructor to initialize any variables needed for the benchmark
    Benchmark(){
        //load inputs from test data and initialze output array -- only set up for 1 test case per file
        meta = *(test_metadata*)meta_array[0];
        vecLen = meta.vec_len;

        output = (int32_t*)vicuna_malloc(2 * vecLen);

        vec_a = (int16_t*)vec_a_array[0];
        vec_b = (int16_t*)vec_b_array[0];
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        mul(vec_a, vec_b, output, vecLen);
        return 0; //always success
    };

    //Validate Output
    int validate_benchmark()
    {
        report_metadata(vecLen);
        int32_t* reference = (int32_t*)vec_c_array[0];
        int code = 0;
        for(uint32_t i=0; i<vecLen; i++)
        {
            if (output[i] != reference[i])
            {
                code = 1;
            }
        }

        if (!(code == 0)) {
            report_diff(output, output, vecLen);
        }
        return code;
    };
    //Cleanup any allocatations
    ~Benchmark(){
        vicuna_free(output);
    };
};

#endif
