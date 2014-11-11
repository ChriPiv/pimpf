#ifndef PIMPF_PROGRAM
#define PIMPF_PROGRAM

#include <string>
#include <vector>
#include "command.h"

namespace pimpfvm {

class Program {

private:
    std::vector<Instruction> Instr_;

public:
    void ReadFromTerminal();
    void ReadFromFile(std::string Filename);
    void Print();
    std::vector<Instruction>& GetInstructions();
    void AddInstruction(Instruction Instr);
    unsigned GetSize();

};

}

#endif // PIMPF_PROGRAM
