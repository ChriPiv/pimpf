#include <assert.h>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include "memory.h"
#include "program.h"
#ifdef PIMPF_AUTOGRADER_RELEASE
#include "pimpf_lib.h"
#endif // PIMPF_AUTOGRADER_RELEASE
#include <iostream>
namespace pimpfvm {

Memory::Memory (unsigned MemorySize) : MemSize_(MemorySize) {

    #ifdef PIMPF_AUTOGRADER_RELEASE
    MemSize_ = 256;
    #else
    MemArray_ = new uint32_t [MemSize_];
    #endif // PIMPF_AUTOGRADER_RELEASE

}

Memory::~Memory() {
    #ifndef PIMPF_AUTOGRADER_RELEASE
    if (MemArray_)
        delete[] MemArray_;
    #endif

}

void Memory::ResetMemory() {

    #ifndef PIMPF_AUTOGRADER_RELEASE
    std::memset (MemArray_, 255, MemSize_);
    #endif

}

void Memory::CopyProgramIntoMemory(Program & Prog) {

    unsigned counter = 0;
    for(auto Instr : Prog.GetInstructions()) {
        #ifdef PIMPF_AUTOGRADER_RELEASE
        pimpf::write_to_memory(counter, InstrToInt(Instr));
        #else
        MemArray_[counter] = InstrToInt(Instr);
        #endif
        counter++;
    }
    #ifdef PIMPF_AUTOGRADER_RELEASE
    pimpf::write_to_memory(counter, 0);
    #else
    MemArray_[counter] = 0;
    #endif

}

uint32_t Memory::Read (unsigned Index) {

    assert(Index >= 0);
    assert(Index < MemSize_);
    #ifdef PIMPF_AUTOGRADER_RELEASE
    return pimpf::read_from_memory(Index);
    #else
    return MemArray_[Index];
    #endif // PIMPF_AUTOGRADER_RELEASE

}

void Memory::Write (unsigned Index, uint32_t Value) {
    
    assert(Index >= 0);
    assert(Index < MemSize_);
    #ifdef PIMPF_AUTOGRADER_RELEASE
    pimpf::write_to_memory(Index, Value);
    #else
    MemArray_[Index] = Value;
    #endif // PIMPF_AUTOGRADER_RELEASE

}

unsigned Memory::GetSize() {

    return MemSize_;

}

bool Memory::IsEmpty(unsigned Start, unsigned End) {

    for (unsigned i=Start; i<=End; i++)
        if(MemArray_[i])
            return false;
    return true;

}

void Memory::DumpMemory() {

    for (unsigned i=0; i<MemSize_; i++) {
        std::cout << "[" << std::hex << std::setfill('0') << std::setw(8) << MemArray_[i] << "]\t";
        if( i%5 == 4)
            std::cout << std::endl;
    }
    std::cout << std::endl << std::setw(0);

}

}
