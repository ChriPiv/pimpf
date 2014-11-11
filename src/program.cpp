#include <assert.h>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <sstream>
#include <vector>
#include "program.h"
#include "command.h"
#ifdef PIMPF_AUTOGRADER_RELEASE
#include "pimpf_lib.h"
#endif // PIMPF_AUTOGRADER_RELEASE
#include <iostream>
namespace pimpfvm {

void Program::ReadFromTerminal() {

    std::uint32_t Input;
    Instruction Instr;

    while (true) {

        std::cin >> std::hex >> Input;
        if (!Input)
            return;
        
        Instr = IntToInstr(Input);
        assert(CommandInfo[Instr.Com].first.length()>0); // Check that the command exists
        // Further info about CommandInfo can be found in command.h
        Instr_.push_back(Instr); 

    }

}

void Program::ReadFromFile(std::string Filename) {

    std::ifstream Ifs(Filename);
    std::string Str((std::istreambuf_iterator<char>(Ifs)),
                     std::istreambuf_iterator<char>());

    std::istringstream Iss(Str);
    std::string Word;
    std::uint32_t Input;
    Instruction Instr;

    while (Iss >> Word) {

        if (Word == "0")
            break;

        Input = std::stol(Word, nullptr, 16);
        Instr = IntToInstr(Input);
        assert(CommandInfo[Instr.Com].first.length()>0); // Check that the command exists
        // Further info about CommandInfo can be found in command.h
        Instr_.push_back(Instr);

    }

}

void Program::Print() {

    #ifdef PIMPF_AUTOGRADER_RELEASE
    for(auto Instr : Instr_) 
        pimpf::print_decoded_instruction((unsigned)Instr.Com, Instr.Param1, Instr.Param2, Instr.Param3);

    for(auto Instr : Instr_) {

        pimpf::disassembly << CommandInfo[Instr.Com].first;

        if ( CommandInfo[Instr.Com].second > 0 )
            pimpf::disassembly << " " << (unsigned)Instr.Param1;
        if ( CommandInfo[Instr.Com].second > 1 )
            pimpf::disassembly << ", " << (unsigned)Instr.Param2;
        if ( CommandInfo[Instr.Com].second > 2 )
            pimpf::disassembly << ", " << (unsigned)Instr.Param3;
        pimpf::disassembly << "\n";
    }

    #else
    
    for(auto Instr : Instr_) {

        std::cout << CommandInfo[Instr.Com].first << " ";

        if ( CommandInfo[Instr.Com].second > 0 )
            std::cout << (int)Instr.Param1 << " ";
        if ( CommandInfo[Instr.Com].second > 1 )
            std::cout << (int)Instr.Param2 << " ";
        if ( CommandInfo[Instr.Com].second > 2 )
            std::cout << (int)Instr.Param3 << " ";

        std::cout << std::endl;
    }
    #endif // PIMPF_AUTOGRADER_RELEASE


}

std::vector<Instruction>& Program::GetInstructions() {

    return Instr_;

}

void Program::AddInstruction(Instruction Instr) {

    Instr_.push_back(Instr);

}

unsigned Program::GetSize() {

    return Instr_.size();

}

}
