#ifndef PIMFP_EXECUTE
#define PIMFP_EXECUTE

#include <cstdint>
#include <iostream>
#include "command.h"
#include "simulation.h"
#include "memory.h"
#ifdef PIMPF_AUTOGRADER_RELEASE
#include "pimpf_lib.h"
#endif // PIMPF_AUTOGRADER_RELEASE

namespace pimpfvm {

Simulation::Simulation(Memory & Mem) : Mem_(Mem) {}

void Simulation::Run(unsigned long MaxCycles) {

    unsigned ProgramCounter = 0;
    unsigned long CycleCounter = 0;
    bool cont = true;
    Instruction Instr;
    uint32_t Input;

    while (cont && ProgramCounter < Mem_.GetSize() && (!MaxCycles || CycleCounter<MaxCycles)) {

        CycleCounter++;
        
        // Fetch + Decode
        Instr = IntToInstr(Mem_.Read(ProgramCounter));
        
        // Execute
        switch(Instr.Com) {

            // General Instructions
            case Command::SET:
                Mem_.Write(Instr.Param1, Instr.Param2 + Instr.Param3*256);
                break;
            case Command::OUT:
                #ifdef PIMPF_AUTOGRADER_RELEASE
                pimpf::print_program_output(Mem_.Read(Instr.Param1));
                #else
                std::cout << (int) Mem_.Read(Instr.Param1) << std::endl;
                #endif // PIMFP_AUTOGRADER_RELEASE
                break;
            case Command::IN:
                std::cin >> std::dec >> Input;
                Mem_.Write(Instr.Param1, Input);
                break;
            case Command::HLT:
                cont = false;
                #ifndef PIMPF_AUTOGRADER_RELEASE
                std::cout << "Program halted." << std::endl;
                #endif
                break;

            // Arithmetic instructions
            case Command::ADD:
                Mem_.Write(Instr.Param3, (Mem_.Read(Instr.Param1) + Mem_.Read(Instr.Param2)) );
                break;
            case Command::SUB:
                Mem_.Write(Instr.Param3, (Mem_.Read(Instr.Param1) - Mem_.Read(Instr.Param2)) );
                break;
            case Command::MUL:
                Mem_.Write(Instr.Param3, (Mem_.Read(Instr.Param1) * Mem_.Read(Instr.Param2)) );
                break;
            case Command::DIV:
                Mem_.Write(Instr.Param3, (Mem_.Read(Instr.Param1) / Mem_.Read(Instr.Param2)) );
                break;
            case Command::MOD:
                Mem_.Write(Instr.Param3, (Mem_.Read(Instr.Param1) % Mem_.Read(Instr.Param2)) );
                break;

            // Branch Instructions
            case Command::JMP:
                ProgramCounter = Instr.Param1 - 1;
                break;
            case Command::JEQ:
                if(Mem_.Read(Instr.Param2) == Mem_.Read(Instr.Param3))
                    ProgramCounter = Instr.Param1 - 1;
                break;
            case Command::JNE:
                if(Mem_.Read(Instr.Param2) != Mem_.Read(Instr.Param3))
                    ProgramCounter = Instr.Param1 - 1;
                break;
            case Command::JGR:
                if(Mem_.Read(Instr.Param2) > Mem_.Read(Instr.Param3))
                    ProgramCounter = Instr.Param1 - 1;
                break;
            case Command::JLS:
                if(Mem_.Read(Instr.Param2) < Mem_.Read(Instr.Param3))
                    ProgramCounter = Instr.Param1 - 1;
                break;
            case Command::JGE:
                if(Mem_.Read(Instr.Param2) >= Mem_.Read(Instr.Param3))
                    ProgramCounter = Instr.Param1 - 1;
                break;
            case Command::JLE:
                if(Mem_.Read(Instr.Param2) <= Mem_.Read(Instr.Param3))
                    ProgramCounter = Instr.Param1 - 1;
                break;

            // In the case anything goes wrong
            default:
                cont = false;
                break;
        }

        ProgramCounter++;

    }

}

}

#endif // PIMPF_EXECUTE
