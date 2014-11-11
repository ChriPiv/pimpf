#ifndef PIMPF_SIMULATION
#define PIMPF_SIMULATION

#include <cstdint>
#include "memory.h"

namespace pimpfvm {

class Simulation {

private:
    Memory & Mem_;

public:
    Simulation(Memory & Mem);
    void Run(unsigned long MaxCycles = 0);

};

}

#endif // PIMPF_SIMULATION
