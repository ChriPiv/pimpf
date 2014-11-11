#ifndef PIMPF_COMMAND
#define PIMPF_COMMAND

#include <cstdint>
#include <map>
#include <string>

namespace pimpfvm {

enum class Command {

    ADD = 0x01,
    SUB = 0x02,
    MUL = 0x03,
    DIV = 0x04,
    MOD = 0x05,
    SET = 0x06,
    JMP = 0x10,
    JGE = 0x11,
    JLE = 0x12,
    JGR = 0x13,
    JLS = 0x14,
    JEQ = 0x15,
    JNE = 0x16,
    OUT = 0x21,
    IN = 0x22,
    HLT = 0x30

};

struct Instruction {
    Command Com;
    uint8_t Param1, Param2, Param3;
};

Instruction IntToInstr (uint32_t Param);
uint32_t InstrToInt (Instruction Param);

// Gives the Name and the parameter count for each command
static std::map<Command, std::pair<std::string, int>> CommandInfo = {

    {Command::ADD,   std::pair<std::string, int>("add",  3)},
    {Command::SUB,   std::pair<std::string, int>("sub",  3)},
    {Command::MUL,   std::pair<std::string, int>("mul",  3)},
    {Command::DIV,   std::pair<std::string, int>("div",  3)},
    {Command::MOD,   std::pair<std::string, int>("mod",  3)},
    {Command::SET,   std::pair<std::string, int>("set",  3)},
    {Command::JMP,   std::pair<std::string, int>("jmp",  1)},
    {Command::JGE,   std::pair<std::string, int>("jge",  3)},
    {Command::JLE,   std::pair<std::string, int>("jle",  3)},
    {Command::JGR,   std::pair<std::string, int>("jgr",  3)},
    {Command::JLS,   std::pair<std::string, int>("jls",  3)},
    {Command::JEQ,   std::pair<std::string, int>("jeq",  3)},
    {Command::JNE,   std::pair<std::string, int>("jne",  3)},
    {Command::OUT,   std::pair<std::string, int>("out",  1)},
    {Command::IN,    std::pair<std::string, int>("in",   1)},
    {Command::HLT,   std::pair<std::string, int>("hlt",  0)}

    // Note: if you try to access an invalid command, the map will return the
    // default constructor of std::pair<std::string, int>, therefore
    // "" and 0

};

}

#endif // PIMPF_COMMAND
