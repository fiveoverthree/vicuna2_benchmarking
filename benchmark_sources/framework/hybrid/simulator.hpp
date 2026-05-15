#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP 

#include <cstdint>
#include "uart.hpp"


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
        *signal_addr = 0x0; //Write to trigger report of current cycle count
    };

    //Function to end measurement
    //Verilator Simulation uses simulated CSRs
    inline void end_measurement()
    {
        *signal_addr = 0x0; //Write to trigger report of current cycle count
    };

    //Termination success or failure function for this simulator
    void terminate(int valid)
    {
        if (valid)
        {
            *terminate_addr = 0x0; //Write 0x0 to signal success
            while(true);//TODO: Use gem5 termination
        } else {
            *terminate_addr = 0xF; //Write 0xF to signal success
            while(true);//TODO: Use gem5 termination
        }
    };

    //Cleanup any allocatations
    ~Simulator(){};
};
#endif
