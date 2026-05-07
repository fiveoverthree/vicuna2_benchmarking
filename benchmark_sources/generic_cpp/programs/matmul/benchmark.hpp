#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP 

#include <cstdint>
//BSP includes
#include "uart.hpp"
//Includes for benchmark
#include "test_data.hpp"
#include "matmul.hpp"

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    test_metadata meta;
    uint32_t numRowsA;
    uint32_t numColsA;
    uint32_t numColsB;

    int32_t *output;

    int8_t* mat_a;
    int8_t* mat_b;

    void report_diff(int32_t* output, int32_t* reference, uint32_t rows, uint32_t cols)
    {
        printf("Your Result:\n\n");
        for(uint32_t i=0; i<rows; i++)
        {
            for(uint32_t j=0; j<cols; j++)
            {
                printf("%d ",output[i*cols + j]);
            }
            printf("\n");
        }
        printf("\n");
        printf("Reference Result:\n\n");
        for(uint32_t i=0; i<rows; i++)
        {
            for(uint32_t j=0; j<cols; j++)
            {
                printf("%d ",reference[i*cols + j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    void report_metadata(uint32_t numRowsA, uint32_t numColsA, uint32_t numColsB)
    {
        printf("Testcase:\n\n");
        printf("Matrix A [%d x %d] X Matrix B [%d x %d]:\n\n", numRowsA, numColsA, numColsA, numColsB);

    }

    public:
    /*
    * Required Functions for test framework
    */
    //override constructor to initialize any variables needed for the benchmark
    Benchmark(){
        //load inputs from test data and initialze output array -- only set up for 1 test case per file
        meta = *(test_metadata*)meta_array[0];
        numRowsA = meta.numRowsA;
        numColsA = meta.numColsA;
        numColsB = meta.numColsB;

        output = (int32_t*)vicuna_malloc(4 * numRowsA * numColsB);

        mat_a = (int8_t*)mat_a_array[0];
        mat_b = (int8_t*)mat_b_array[0];
    };

    //Call code to be benchmarked
    inline bool run_benchmark()
    {
        matmul(mat_a, mat_b, output, numColsA, numRowsA, numColsB);
        return true; //always success
    };

    //Validate Output
    bool validate_benchmark()
    {
        report_metadata(numRowsA, numColsA, numColsB);
        int32_t* reference = (int32_t*)mat_c_array[0];
        bool match = true;
        for(uint32_t i=0; i<numRowsA; i++)
        {
            for(uint32_t j=0; j<numColsB; j++)
            {
                if (output[i*numColsB + j] != reference[i*numColsB + j])
                {
                    match = false;
                }
            }
        }

        if (!match) {
            report_diff(output, reference, numRowsA, numColsB);
        }
        return match;
    };
    //Cleanup any allocatations
    ~Benchmark(){
        vicuna_free(output);
    };
};

#endif
