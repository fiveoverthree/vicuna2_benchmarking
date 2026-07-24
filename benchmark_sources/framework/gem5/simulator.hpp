#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP 

#include <cstdint>
#include <gem5/m5ops.h>

#include <cstdint>
class Simulator
{
    private:

    // Stats variables
    uint32_t start_cycles, start_instr;
    uint32_t end_cycles, end_instr;

    public:

    Simulator(){};

    //Function to start measurement
    //Verilator Simulation uses simulated CSRs
    inline void begin_measurement()
    {
        m5_reset_stats(0, 0);
    };

    //Function to end measurement
    //Verilator Simulation uses simulated CSRs
    inline void end_measurement()
    {
        m5_dump_stats(0, 0);
    };

    //Termination success or failure function for this simulator
    int terminate(int code)
    {
        if (code)
        {
            m5_fail(0, code); //TODO: ctest reports "test passed", but output says "fail instruction encountered"
        }
        m5_exit(0); //exit normally
        return code; 
    };

    //Cleanup any allocatations
    ~Simulator(){};
};
#endif
