#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP 

#include <cstdint>


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
    };

    //Function to end measurement
    //Verilator Simulation uses simulated CSRs
    inline void end_measurement()
    {

    };

    //Termination success or failure function for this simulator
    int terminate(int valid)
    {
        // if (valid)
        // {
        //     *terminate_addr = 0x0; //Write 0x0 to signal success
        // } else {
        //     *terminate_addr = 0xF; //Write 0xF to signal success
        // }
    };

    //Cleanup any allocatations
    ~Simulator(){};
};
#endif
