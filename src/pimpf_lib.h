#ifndef PIMPF_LIB
#define PIMPF_LIB

#include <sstream>

namespace pimpf {

extern std::stringstream disassembly;

void write_to_memory (const int address, const unsigned int value);

unsigned int read_from_memory (const int address);

void print_decoded_instruction (unsigned int opcode, unsigned int op1, unsigned int op2, unsigned int op3);

void print_program_output (const unsigned int value);

}

#endif // PIMPF_LIB
