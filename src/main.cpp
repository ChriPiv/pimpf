#include <iostream>
#include <string>
#include <vector>

#include "command.h"
#include "memory.h"
#include "program.h"
#include "simulation.h"

int main(int argc, char *argv[]) {

    #ifndef PIMPF_AUTOGRADER_RELEASE
    /****************************************/
    // Parse konsole input
    /****************************************/
    bool FlagPrint = false;
    bool FlagTerminalInput = false;
    bool FlagDumpMemory = false;
    std::string Filename = "";
    if (argc < 2) {   
        std::cout << "Usage: pimpf [-p] [-t] [-d] filename" << std::endl;
        std::cout << "[-p]: Print the disassembled program before executing" << std::endl;
        std::cout << "[-t]: Read the program input from the terminal instead from a file" << std::endl;
        std::cout << "[-d]: Dump the memory after having executed the program." << std::endl;
        return 0;
    }
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == "-p")
            FlagPrint = true;
        else if (std::string(argv[i]) == "-t")
            FlagTerminalInput = true;
        else if (std::string(argv[i]) == "-d")
            FlagDumpMemory = true;
        else
            Filename = std::string(argv[i]);
    }

    if(!FlagTerminalInput && Filename.length() < 2) {
        std::cout << "Please specify filename." << std::endl;
        return 0;
    }
    #endif // AUTOGRADER_RELEASE


    /****************************************/
    // Load the program
    /****************************************/
    pimpfvm::Program Prog;
    #ifdef PIMPF_AUTOGRADER_RELEASE
    Prog.ReadFromTerminal();
    #else
    if(FlagTerminalInput)
        Prog.ReadFromTerminal();
    else
        Prog.ReadFromFile(Filename);
    #endif // AUTOGRADER_RELEASE


    /****************************************/
    // Initialize the VM memory
    /****************************************/
    pimpfvm::Memory Mem(256);
    Mem.ResetMemory();
    Mem.CopyProgramIntoMemory(Prog);


    /****************************************/
    // Print the program
    /****************************************/
    #ifndef PIMPF_AUTOGRADER_RELEASE
    if (FlagPrint)
    #endif // AUTOGRADER_RELEASE
    {
        Prog.Print();
    }


    /****************************************/
    // Run the VM
    /****************************************/
    pimpfvm::Simulation Sim(Mem);
    Sim.Run();
    #ifndef PIMPF_AUTOGRADER_RELEASE
    if (FlagDumpMemory)
        Mem.DumpMemory();
    if (Mem.IsEmpty(Prog.GetSize(), 255))
        std::cout << "Program emptied memory." << std::endl;
    #endif

    return 0;

}
