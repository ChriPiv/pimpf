#ifndef PIMPF_MEMORY
#define PIMPF_MEMORY

#include <cstdint>
#include "program.h"

namespace pimpfvm {

class Memory {

private:
    uint32_t *MemArray_;
    unsigned MemSize_;

public:
    Memory (unsigned MemorySize=256);
    ~Memory();
    void ResetMemory();
    void CopyProgramIntoMemory(Program & Prog);
    uint32_t Read(unsigned Index);
    void Write(unsigned Index, uint32_t Value);
    unsigned GetSize();
    bool IsEmpty(unsigned Start, unsigned End);
    void DumpMemory();

};

}

#endif // PIMPF_MEMORY
