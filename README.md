This is the virtual machine I wrote for the pimpf contest (specifications: http://lec.inf.ethz.ch/ifmp/2014/pimpf/pimpf.pdf). It has two modes, the normal mode (where I take care of memory management and printing) and autograder release mode (where I actually use the pimpf_lib.cpp. Both modes are built when simply using "make". The output can be found in the "bin" folder.

"test" contains some programs to test the vm with. The Memory_sweeper program is my submission for the contest.

Usage of the programs:
autograder release:
bin/pimpf-autograder-release < filename.cpu

regular release:
bin/pimpf [-p] [-t] [-d] filename.cpu
The flag -p makes pimpf print the disassembled program before executing
The flag -t makes pimpf read the program from the cli instead of a file
The flag -d makes pimpf dump the memory after execution

Therefore to test my sweeping program you can use one of the following commands:
bin/pimpf-autograder-release < test/Memory_sweep_14-923-544.cpu
bin/pimpf -t < test/Memory_sweep_14-923-544.cpu
