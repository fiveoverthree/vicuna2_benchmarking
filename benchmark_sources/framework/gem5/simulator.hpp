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
    int terminate(int code)
    {
        //pass return code, gem5 uses return code of main() to signal success/failure
        return code; 
    };

    //Cleanup any allocatations
    ~Simulator(){};
};
#endif
