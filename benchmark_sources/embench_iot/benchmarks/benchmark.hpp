#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

extern "C"
{
    #include "support.h"
}

class Benchmark
{
    private:
    /*
    * Private Helper Functions and variables
    */
    int result;

    public:
    /*
    * Required Functions for test framework
    */
    Benchmark(){
        // filter initialization
        initialise_benchmark();
    };

    //Call code to be benchmarked
    inline int run_benchmark()
    {
        this->result = benchmark();
        return 0; //cannot fail internally

    };

    //Validate Output
    int validate_benchmark()
    {
        // embench-iot returns 1 on success
        if(verify_benchmark(this->result))
        {
            return 0;
        }
        return 1;
    };
    //Cleanup any allocatations
    ~Benchmark(){
    };
};

#endif // BENCHMARK_HPP
