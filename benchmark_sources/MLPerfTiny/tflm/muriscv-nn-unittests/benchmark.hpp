#ifndef AWW_INT8_BENCHMARK_HPP
#define AWW_INT8_BENCHMARK_HPP
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>


#ifdef __cplusplus
extern "C" {
#endif

int muriscv_nn_main(void);

#ifdef __cplusplus
}
#endif

class Benchmark{
    private: 
    public:
    Benchmark(){};

    inline int run_benchmark(){
        Simulator simulator;
        simulator.begin_measurement();
        int ret = muriscv_nn_main();
        simulator.end_measurement();
        simulator.terminate(ret);
        return 0;
    }

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


