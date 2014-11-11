CC=g++
CFLAGS=-std=c++11 -Wall -O3

all: vm vm-autograder-release

vm: src/command.o src/memory.o src/program.o src/simulation.o
	$(CC) $(CFLAGS) src/main.cpp -o bin/pimpf src/command.o src/memory.o src/program.o src/simulation.o
    
vm-autograder-release: src/command.o src/memory-autograder.o src/pimpf_lib.o src/program-autograder.o src/simulation-autograder.o
	$(CC) $(CFLAGS) -DPIMPF_AUTOGRADER_RELEASE src/main.cpp -o bin/pimpf-autograder-release src/command.o src/memory-autograder.o src/pimpf_lib.o src/program-autograder.o src/simulation-autograder.o


src/command.o:  src/command.cpp
	$(CC) $(CFLAGS) -c src/command.cpp -o src/command.o

src/memory.o: src/memory.cpp
	$(CC) $(CFLAGS) -c src/memory.cpp -o src/memory.o
    
src/memory-autograder.o: src/memory.cpp
	$(CC) $(CFLAGS) -DPIMPF_AUTOGRADER_RELEASE -c src/memory.cpp -o src/memory-autograder.o

src/pimpf_lib.o: src/pimpf_lib.cpp
	$(CC) $(CFLAGS) -c src/pimpf_lib.cpp -o src/pimpf_lib.o

src/program.o: src/program.cpp
	$(CC) $(CFLAGS) -c src/program.cpp -o src/program.o
    
src/program-autograder.o: src/program.cpp
	$(CC) $(CFLAGS) -DPIMPF_AUTOGRADER_RELEASE -c src/program.cpp -o src/program-autograder.o

src/simulation.o: src/simulation.cpp
	$(CC) $(CFLAGS) -c src/simulation.cpp -o src/simulation.o
    
src/simulation-autograder.o: src/simulation.cpp
	$(CC) $(CFLAGS) -DPIMPF_AUTOGRADER_RELEASE -c src/simulation.cpp -o src/simulation-autograder.o


clean:
	rm bin/pimpf bin/pimpf-autograder-release src/command.o src/memory.o src/memory-autograder.o src/pimpf_lib.o src/program.o src/program-autograder.o src/simulation.o src/simulation-autograder.o
