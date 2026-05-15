#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP 

#include <cstdint>
//BSP includes
//#include "uart.hpp"
//Includes for benchmark
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "toycar_int8_data/toycar_int8_input_data.h"
#include "toycar_int8_data/toycar_int8_model_data.h"
#include "toycar_int8_data/toycar_int8_model_settings.h"
#include "toycar_int8_data/toycar_int8_output_data_ref.h"

//Due to scoping/dynamic allocation issue with tflm, unfortunately need to include simulator specific  in this file
#include "simulator.hpp"

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    static constexpr size_t tensor_arena_size = 256 * 1024;
    alignas(16) uint8_t tensor_arena[tensor_arena_size];

    const tflite::Model *model;
    tflite::MicroInterpreter *interpreter_ptr;

    public:
    /*
    * Required Functions for test framework
    */
    //
    Benchmark(){
        };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        //Due to scoping issue for the tflite::MicroInterpreter, init must be inside here.  Fixes to dynamic allocation should allow this to be separated correctly
        model = tflite::GetModel(toycar_int8_model_data);
        static tflite::MicroMutableOpResolver<1> resolver;
        resolver.AddFullyConnected();
        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, tensor_arena_size);
        if (interpreter.AllocateTensors() != kTfLiteOk)
        {
            return 1;
            //uart_printf("Failed to allocate tensors!\n");
        }
        memcpy(interpreter.input(0)->data.int8, (int8_t *)toycar_int8_input_data[0], toycar_int8_input_data_len[0]); //Only one test case

        Simulator simulator; //need to initialize simulator internally
        //Restart measurement
        simulator.begin_measurement();

        if (interpreter.Invoke() != kTfLiteOk)
        {
            //uart_printf("Failed in Invoke!\n");
            return 2;
        } 
        //End measurement for real
        simulator.end_measurement();


        // Due to scoping issue, validation also inside this function
        int32_t sum = 0;
        for (size_t j = 0; j < toycar_int8_input_data_len[0]; j++)
        {
            int32_t diff1 = (int8_t)toycar_int8_input_data[0][j] - (int8_t)interpreter.output(0)->data.int8[j];
            int32_t square = diff1*diff1;
            sum += square;
        }
        sum /= toycar_int8_input_data_len[0];

        int32_t diff = abs(sum - toycar_int8_output_data_ref[0]);
        
        if (diff > 1)
        {
            //uart_printf("ERROR: at #%d, sum %d ref %d diff %d \n", 0, sum, toycar_int8_output_data_ref[0], diff);
            simulator.terminate(false); //Terminate simulation instead of returning (due to scoping problem)
            return 3;
        }
        else
        {
            //uart_printf("Sample #%d pass, sum %d ref %d diff %d \n", 0, sum, toycar_int8_output_data_ref[0], diff);
            simulator.terminate(true); //Terminate simulation instead of returning (due to scoping problem)
            return 0;
        }

    };

    //Validate Output
    int validate_benchmark()
    {
        //Not used due to scoping issue
        return true;
    };
    //Cleanup any allocatations
    ~Benchmark(){
    };
};
#endif
