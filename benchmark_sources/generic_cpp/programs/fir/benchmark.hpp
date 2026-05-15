#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP 

#include <cstdint>
//BSP includes
#include "uart.hpp"
//Includes for benchmark
#include "test_data.hpp"
#include "fir.hpp"

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    test_metadata meta;
    uint32_t input_len;
    uint32_t filter_len;
    uint32_t filter_channels;

    int32_t *output;

    int16_t* input;
    int16_t* filter;

    void report_diff(int32_t* output, int32_t* reference, uint32_t output_len, uint32_t num_channels)
    {
        uart_printf("Your Result:\n\n");
        for(uint32_t j=0; j<num_channels; j++)
        {
            for(uint32_t i=0; i<output_len; i++)
            {
                uart_printf("%d ",output[j*output_len + i]);
            }
            uart_printf("\n");
        }
        uart_printf("\n");
        uart_printf("Reference Result:\n\n");
        for(uint32_t j=0; j<num_channels; j++)
        {
            for(uint32_t i=0; i<output_len; i++)
            {
                    uart_printf("%d ",reference[j*output_len + i]);
            }
            uart_printf("\n");
        }
        uart_printf("\n");
    }

    void report_metadata(uint32_t output_len, uint32_t filter_len, uint32_t num_channels)
    {
        uart_printf("Testcase:\n\n");
        uart_printf("Multi-Channel FIR Filter\n");
        uart_printf("Input %d elements long\n", output_len);
        uart_printf("Filter %d elements long\n", filter_len);
        uart_printf("%d Channels\n\n", num_channels);
    }

    public:
    /*
    * Required Functions for test framework
    */
    //override constructor to initialize any variables needed for the benchmark
    Benchmark(){
        //load inputs from test data and initialze output array -- only set up for 1 test case per file
        meta = *(test_metadata*)meta_array[0];
        input_len = meta.input_len;
        filter_len = meta.filter_len;
        filter_channels = meta.filter_channels;

        output = (int32_t*)vicuna_malloc((input_len - (filter_len - 1))*4*filter_channels);

        input = (int16_t*)input_array[0];
        filter = (int16_t*)filter_array[0];
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        fir(input, filter, output, input_len, filter_len, filter_channels);
        return 0; //always success
    };
    //Validate Output
    int validate_benchmark()
    {
        report_metadata(input_len, filter_len, filter_channels);
        int32_t* reference = (int32_t*)ref_output_array[0];
        int code = 0;
        for(uint32_t i=0; i<(input_len - (filter_len - 1))*filter_channels; i++)
        {
            if (output[i] != reference[i])
            {
                code = 1;
            }
        }

        if (!(code == 0)) {
            report_diff(output, reference, (input_len - (filter_len - 1)), filter_channels);
        }
        return code;
    };
    //Cleanup any allocatations
    ~Benchmark(){
        vicuna_free(output);
    };
};

#endif
