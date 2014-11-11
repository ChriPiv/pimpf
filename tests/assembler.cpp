#include<iostream>
#include<string>

// POST: returns true iff next instruction from i could be assembled, in which
//       case value is the instruction code (or the trailing 0 if the stream is empty)
bool can_assemble (std::istream& is, unsigned int& value)
{
  std::string str;
  unsigned int ops;
  value = 0;
  if ((is >> str).fail()) {value = 0x00; return true;}
  if      (str == "set")  {value = 0x06; ops = 3;}
  else if (str == "out")  {value = 0x21; ops = 1;}
  else if (str == "in")   {value = 0x22; ops = 1;}
  else if (str == "hlt")  {value = 0x30; ops = 0;}
  else if (str == "add")  {value = 0x01; ops = 3;}
  else if (str == "sub")  {value = 0x02; ops = 3;}
  else if (str == "mul")  {value = 0x03; ops = 3;}
  else if (str == "div")  {value = 0x04; ops = 3;}
  else if (str == "mod")  {value = 0x05; ops = 3;}
  else if (str == "jmp")  {value = 0x10; ops = 1;}
  else if (str == "jge")  {value = 0x11; ops = 3;}
  else if (str == "jle")  {value = 0x12; ops = 3;}
  else if (str == "jgr")  {value = 0x13; ops = 3;}
  else if (str == "jls")  {value = 0x14; ops = 3;}
  else if (str == "jeq")  {value = 0x15; ops = 3;}
  else if (str == "jne")  {value = 0x16; ops = 3;}
  else return false; // illegal opcode

  value *= 0x1000000;
  unsigned int fact = 0x10000;
  while (ops > 0) {
    unsigned int op;
    char ch;
    if ((is >> op).fail())
      return false;
    value += op * fact;
    fact /= 0x100;
    --ops;
    if ((ops > 0) && ((is >> ch).fail() || (ch != ',')))
      return false;
  }
  char comment[255];
  is.getline(comment,255); // skip comments
  return true;
}

int main ()
{
  unsigned int value;
  for (unsigned int count = 0;; ++count) {
    if (can_assemble (std::cin, value)) {
      std::cout << std::hex << value << " ";
      if (value == 0) break;
    }
    else {
      std::cout << "error in instruction " << std::dec << count << ".\n";
      break;
    }
  } 
  std::cout << std::endl;
  return 0;
}

