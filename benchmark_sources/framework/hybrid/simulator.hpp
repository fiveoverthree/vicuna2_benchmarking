#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP 

#include <cstdint>
#include "uart.hpp"
#include <gem5/m5ops.h>


#include <cstdint>
class Simulator
{
    private:

    // Stats variables
    uint32_t start_cycles, start_instr;
    uint32_t end_cycles, end_instr;

    //Memory mapped address to terminate execution
    volatile int* const signal_addr    = ( int*) 0x00000400;
    volatile int* const terminate_addr = ( int*) 0x00000408;

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
    void terminate(int code)
    {
        if (code != 0)
        {
            m5_fail(0, code);
        }
        m5_exit(0); //exit normally
    };

    //Cleanup any allocatations
    ~Simulator(){};
};
#endif
