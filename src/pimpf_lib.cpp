// Date: Oct 29, 2014
// ------------------
// Prog: pimpf_lib.cpp
// IMPORTANT: DO NOT MODIFY THIS FILE. ALL PROGRAMS THAT DO NOT WORK
// WITH AN UNMODIFIED VERSION OF THIS FILE DO NOT RECEIVE ANY POINTS

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cassert>

namespace pimpf {
  // useful constants
  // ----------------
  const int OC = 0x1000000;
  const int O1 = 0x10000;
  const int O2 = 0x100;

  // output streams
  // --------------
  std::stringstream out;         // for printing program output
  std::stringstream dec;         // for printing decoding output
  std::stringstream pout;        // for verifying program output
  std::stringstream dout;        // for verifying decoding output
  std::stringstream disassembly; // for printing and verifying 
                                 // disassembled instructions

  // PIMPF's memory: cells will be initialized with 0xffffffff in Verify()
  // ---------------------------------------------------------------------
  unsigned int memory [256]; 

  // program
  // -------
  std::vector<unsigned int> loaded_program;
  bool loading = true;

  // memory access functions 
  // -----------------------

  // PRE: 0 <= address < 256
  // POST: value is written to address
  void write_to_memory (const int address, const unsigned int value)
  {
    assert (0 <= address && address < 256);
    memory[address] = value;
    // the values before the first 0 form the loaded program
    if (loading) {
      if (value == 0)
	loading = false;
      else
	loaded_program.push_back (value);
    }
  }
 
  // PRE: 0 <= address < 256
  // POST: value a address is returned
  unsigned int read_from_memory (const int address)
  {
    assert (0 <= address && address < 256);
    return memory[address];
  }

  // POST: the memory content is written to std::cout in hexadecimal
  //       format, starting from address 0, outputting 8 values per row
  void dump_memory ()
  {
    std::cout << "Memory after your program execution:\n";
    std::cout << "------------------------------------\n";
    for (int address = 0; address < 256;) {
      std::cout << '[' << std::hex << std::setw(8) << memory [address]
		<< ']';
      if (++address % 8 == 0) std::cout << '\n';
    }
    std::cout << std::endl;
  }

  // program output function, to be called in step (d)
  // -------------------------------------------------
  void print_program_output (const unsigned int value) 
  {
    // output for user
    out << "out> " << value << std::endl;
    // output for verifyer
    pout << value << " ";
  }

  // output of decoded instruction, to be called in step (b)
  // -------------------------------------------------------
  void print_decoded_instruction (unsigned int opcode, 
				  unsigned int op1, 
				  unsigned int op2, 
				  unsigned int op3)
  {
    // output for user
    dec <<  "dec> opcode= " << opcode 
	<< ", ops= " << op1 << ", " << op2 << ", " <<op3 << "\n";
    // output for verifyer
    dout << std::hex << opcode * OC + op1 *O1 + op2 * O2 + op3 << " ";
  }

  // class for test programs
  // -----------------------
  struct Program {
    std::string name;   // program name
    std::string code;   // program code
    std::string output; // expected output
  };

  // The actual test programs
  // ------------------------
  Program Output = {
    /*
      0: set 10, 42 // a = 42
      1: out 10     // output a
      2: hlt        // stop
    */
    "Output",
    "60a2a00 210a0000 30000000",
    "42"
  };

  Program Adder = {
    /*
      0: set 10, 2      // a = 2
      1: set 11, 3      // b = 3
      2: add 10, 11, 10 // a = a + b
      3: out 10         // output a
      4: hlt            // stop
    */
    "Adder",
    "60a0200 60b0300 10a0b0a 210a0000 30000000",
    "5"
  };

  Program Loop = {
    /*
      0: set 10, 10     // i = 10
      1: set 11, 0      // constant 0
      2: set 12, 1      // constant 1
      3: jeq 7, 10, 11  // if i == 0 goto 7
      4: out 10         // output i
      5: sub 10, 12     // i = i - 1
      6: jmp 3          // goto 3
      7: hlt            // stop
    */
    "Loop",
    "60a0a00 60b0000 60c0100 15070a0b 210a0000 020a0c0a 10030000\
       30000000",
    "10 9 8 7 6 5 4 3 2 1"
  };

  Program Gcd = {
    /*
      set 10, 47, 4       // 0: a = 1071
      set 11, 5, 4        // 1: b = 1029
      set 12, 0, 0        // 2: c = 0
      jeq 8, 11, 12       // 3: if b = 0 goto 8
      mod 10, 11, 13      // 4: h = a mod b
      add 11, 12, 10      // 5: a = b
      add 13, 12, 11      // 6: b = h
      jmp 3               // goto 3
      out 10              // print a
      hlt                 // halt
    */
    "Gcd",
    "60a2f04 60b0504 60c0000 15080b0c 50a0b0d 10b0c0a 10d0c0b 10030000\
       210a0000 30000000",
    "21"
  };

  Program Gcd_with_input = {
    /*
      in 10
      in 11
      set 12, 0, 0
      jeq 8, 11, 12
      mod 10, 11, 13
      add 11, 12, 10
      add 13, 12, 11
      jmp 3
      out 10
      hlt
    */
    "Gcd_with_input",
    "220a0000 220b0000 60c0000 15080b0c 50a0b0d 10b0c0a 10d0c0b 10030000\
       210a0000 30000000",
    "21"
  };

  Program Primes = {
    /* Essentially the Spaghetti code from the lecture
       set 18, 0, 0      // 0:   constant 0
       set 19, 1, 0      // 1:   constant 1
       set 17, 255, 0    // 2:   constant 255
       set 20, 2, 0      // 3:   10: N = 2
       set 21, 1, 0      // 4:   20: D = 1
       add 21, 19, 21    // 5:   30: D = D + 1
       jeq 13, 20, 21    // 6:   40: IF N = D GOTO 100
       mod 20, 21, 22    // 7:   50: Q = N MOD D
       jeq 10, 22, 18    // 8:   55: IF Q = 0 GOTO 70 
       jmp 5             // 9:   60: GOTO 30
       add 20, 19, 20    // 10:  70: N = N + 1
       jeq 15, 20, 17    // 11:  75: IF N = 255 GOTO 120
       jmp 4             // 12:  80: GOTO 20
       out 20            // 13:  100: PRINT N
       jmp 10            // 14:  110: GOTO 70 
       hlt               // 15:  120:
    */
    "Primes",
    "6120000 6130100 611ff00 6140200 6150100 1151315 150d1415 5141516\
       150a1612 10050000 1141314 150f1411 10040000 21140000 100a0000\
       30000000",
    "2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89\
       97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173 179\
       181 191 193 197 199 211 223 227 229 233 239 241 251"
  };

  class Verify
  {
    // the test programs that can automatically be verified
    std::vector<Program> test_programs;

    // PRE: step is 'a', 'b', 'c', 'd' or 'e'
    // POST: if ok, step is reported as passed, otherwise as not passed
    void pass_msg (bool ok, char step) const
    {
      std::cout << "Step (" << step << ")";
      if (ok)
	std::cout << " passed.\n";
      else
	std::cout << " not (yet) passed.\n";
    }

    // PRE:  p is in test_programs
    // POST: verify step (a): returns true if program p was loaded 
    bool loaded (const Program p) const
    {
      std::stringstream code;
      code << p.code;
      unsigned int value;
      unsigned int address = 0;
      while (!(code >> std::hex >> value).fail()) {
	if (address >= loaded_program.size())
	  return false; // p longer than loaded program
	if (value != loaded_program [address])
	  return false; // p differs from loaded program
	++address;
      }
      return (address == loaded_program.size());
    }
    
    // PRE:  p is in test_programs
    // POST: verify step (b): tells the user whether the program p was
    //       correctly decoded
    void check_decoding (const Program p) const
    {
      std::stringstream code;
      code << p.code;
      unsigned int observed;
      unsigned int expected;
      bool b1;
      bool b2;
      do
        {
	  b1 = !(code >> std::hex >> expected).fail() && (expected != 0);
	  b2 = !(dout >> std::hex >> observed).fail() && (observed != 0);
        }
      while (b1 && b2 && (expected == observed));
      pass_msg (!b1 && !b2,'b');
    }

    // PRE:  p is in test_programs
    // POST: verify step (c): tells the user whether the program p was
    //       correctly disassembled
    void check_disassembly (const Program p) const
    {
      std::stringstream code;
      code << p.code;
      std::string obss = disassembly.str();
      std::stringstream d;
      d << obss;
      bool b1;
      bool b2;
      unsigned int observed;
      unsigned int expected;
      do
        {
	  b1 = assemble (d, observed) && (observed != 0);
	  b2 = !(code >> std::hex >> expected).fail() && (expected != 0);
        }
      while (b1 && b2 && (expected == observed));
      pass_msg (!b1 && !b2,'c');
    }

    // PRE:  p is in test_programs
    // POST: verify step (d): tells the user whether the program p was
    //       correctly run in the sense that it generates the correct
    //       output
    void check_output (const Program p) const
    {
      std::stringstream output;
      output << p.output;
      int observed;
      int expected;
      bool b1;
      bool b2;
      do
        {
	  b1 = !(output >> observed).fail();
	  b2 = !(pout >> expected).fail();
        }
      while (b1 && b2 && (expected == observed));
      pass_msg(!b1 && !b2,'d');
    }

    // POST: verify step (e): tells the user whether the memory starting
    //       from the address after the program has been cleared, and 
    //       if so, returns the size of the program
    void check_memory_sweep () const
    {
      unsigned int address = loaded_program.size();
      for (; address < 256; ++address)
	if (read_from_memory (address) > 0) break;
      std::cout << "Number of program instructions: " 
		<< std::dec << loaded_program.size() << "\n";
      std::cout << "-------------------------------\n";
      pass_msg (address == 256, 'e');	
    }

    // POST: print user decoding as part of autograder output
    void print_dec() const
    {
      std::cout << "Your decoding:" << std::endl;
      std::cout << "--------------" << std::endl;
      std::cout << dec.str();
      std::cout << std::endl;
    }
    void print_out() const
    {
      std::cout << "Your program output:" << std::endl;
      std::cout << "--------------------" << std::endl;
      std::cout << out.str();
      std::cout << std::endl;
    }
 
    bool assemble (std::stringstream & s, unsigned int & value) const
    {
      std::string str;
      unsigned int ops;
      value = 0;
      if ((s >> str).fail())
	return false;
      if      (str == "set") {value = 0x06; ops = 3;}
      else if (str == "out") {value = 0x21; ops = 1;}
      else if (str == "in")  {value = 0x22; ops = 1;}
      else if (str == "hlt") {value = 0x30; ops = 0;}
      else if (str == "add") {value = 0x01; ops = 3;}
      else if (str == "sub") {value = 0x02; ops = 3;}
      else if (str == "mul") {value = 0x03; ops = 3;}
      else if (str == "div") {value = 0x04; ops = 3;}
      else if (str == "mod") {value = 0x05; ops = 3;}
      else if (str == "jmp") {value = 0x10; ops = 1;}
      else if (str == "jge") {value = 0x11; ops = 3;}
      else if (str == "jle") {value = 0x12; ops = 3;}
      else if (str == "jgr") {value = 0x13; ops = 3;}
      else if (str == "jls") {value = 0x14; ops = 3;}
      else if (str == "jeq") {value = 0x15; ops = 3;}
      else if (str == "jne") {value = 0x16; ops = 3;}
      else return false;

      value *= 0x1000000;
      unsigned int fact = 0x10000;
      while (ops > 0)
        {
	  unsigned int op;
	  char ch;
	  if ((s >> op).fail())
	    return false;
	  value += op * fact;
	  fact /= 0x100;
	  --ops;
	  if ((ops > 0) && ((s >> ch).fail() || (ch != ',')))
	    return false;
        }
      return true;
    }

    void print_asm() const
    {
      std::string obss = disassembly.str();
      std::stringstream d;
      d << obss;
      unsigned int value;

      bool first = true;
      while (!d.eof()) {
	std::string line;
	std::getline(d, line);
	if (!line.empty()) {
	  if (first) {
	    std::cout 
	      << "Your disassembly         [and my assembly of it]:" 
	      << std::endl;
	    std::cout 
	      << "-------------------------------------------------" 
	      << std::endl;
	    first = false;
	  }
	  std::cout << "asm> " << line;
	  std::stringstream scan;
	  scan << line;
	  unsigned int pos = line.length();
	  while (pos < 20) { // tab
	    std::cout << " ";
	    pos++;
	  }
	  if(assemble (scan, value))
	    std::cout << "[" << std::hex << std::setw(8) << value << "]";
	  else
	    std::cout << "[" << "could not assemble]";
	  std::cout << "\n";
	}
      }
      if (!first)
	std::cout << "\n";
    }

  public:
    Verify()
    {
      for (int address=0; address<256; ++address) {
	memory[address] = 0xffffffff;
      }
      test_programs.push_back (Output);
      test_programs.push_back (Adder);
      test_programs.push_back (Loop);
      test_programs.push_back (Gcd);
      test_programs.push_back (Gcd_with_input);
      test_programs.push_back (Primes);
    }

    void Error(const std::string error)
    {
      std::cerr << "pimpf autograder error " << error << "\n";
    }

    ~Verify()
    {
      std::cout
	<< "------------------------------------------------------------\n"
        << "The following output has been generated by PIMPF autograder.\n"
        << "------------------------------------------------------------\n"
	<< std::endl;

      // decoding 
      print_dec ();

      // disassembly, plus assembly of it
      print_asm ();

      // program execution
      print_out ();

      // verification
      bool test_program_loaded = false;
      for (std::vector<Program>::iterator 
	     it = test_programs.begin(); it < test_programs.end(); it++) {
	if (loaded (*it)) {
	  test_program_loaded = true;
	  std::cout << "Detected program: " << (*it).name << "\n";
	  std::cout << "----------------- " << "\n";
	  std::cout << "Step (a) passed." << "\n";
	  check_decoding (*it);     // step (b)
	  check_disassembly (*it);  // step (c)
	  check_output (*it);       // step (d)
	  break;
	}
      };
      if (!test_program_loaded) {
	dump_memory ();
	check_memory_sweep();       // step (e)
      }
      std::cout << std::endl;
    }
  } verify;
}
// IMPORTANT: DO NOT MODIFY THIS FILE. ALL PROGRAMS THAT DO NOT WORK
// WITH AN UNMODIFIED VERSION OF THIS FILE DO NOT RECEIVE ANY POINTS
