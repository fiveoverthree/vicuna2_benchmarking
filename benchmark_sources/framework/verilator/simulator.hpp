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
    volatile int* const terminate_addr = ( int*) 0x00000408;

    public:

    Simulator(){};

    //Function to start measurement
    //Verilator Simulation uses simulated CSRs
    inline void begin_measurement()
    {
        asm volatile ("csrr %0,mcycle"   : "=r" (start_cycles)  );
        asm volatile ("csrr %0,minstret"   : "=r" (start_instr)  );
    };

    //Function to end measurement
    //Verilator Simulation uses simulated CSRs
    inline void end_measurement()
    {
        asm volatile ("csrr %0,mcycle"   : "=r" (end_cycles)  );
        asm volatile ("csrr %0,minstret"   : "=r" (end_instr)  );
        uart_printf("Total Cycles:       %d\n",end_cycles-start_cycles);
        uart_printf("Total Instructions: %d\n\n",end_instr-start_instr);
    };

    //Termination success or failure function for this simulator
    void terminate(bool valid)
    {
        if (valid)
        {
            *terminate_addr = 0x0; //Write 0x0 to signal success
        } else {
            *terminate_addr = 0xF; //Write 0xF to signal success
        }
    };

    //Cleanup any allocatations
    ~Simulator(){};
};
#endif
