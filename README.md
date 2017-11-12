# 367Project3
For this assignment, you are going to use C to implement a simulator for the Sequential Y86 architecture. Your input will be a text file that contains first the program counter followed by the memory. A front end is already written that will read this file into an array where each byte of memory is a char. Starting at the memory address given to you as the starting program counter, you will read in bytes and execute the program. This is very similar to what we did in class where we ‘disassembled’ the input and looked at what happens in the different stages for each type of instruction.


For example, consider the partial input given in the box below. The first number (0x0) is the
starting program counter PC. Other values can be used if the program execution can start
elsewhere. The rest of the input is the contents of memory starting at address 0x0. This is what
you will be executing. You start by examining the first byte and then all needed additional bytes
for that instruction. After execution (which may modify the PC), you execute the instruction at
PC, … Start by understanding what the below program should do if simulated.

0x0
30F65704000000000000D06FE0EF6800
000000000000406E7000000000000000
E0EF7000000000000000500E68000000
00000000D00F6006D06F733600000000
0000002406D06F6168D08F2589D09F21
89D09F409E6800000000000000E0EF68
0000000000000000CAFEBABE00000000
341200000000


You will need to implement all of the givenY86 plus some additional instructions to allow you to
dump the contents of registers and memory:
 printall Single byte instruction 0xC0 – prints out the values stored in all 15 registers
 printreg reg – Two byte instruction 0xD0, 0xrA:F – print out the value of the register
indicated by rA
 printmem disp(reg) – 10 byte instruction 0xE0, 0xrA:F, 8 byte displacement – print out
the 8 byte constant stored at address contents(rA) + displacement.
There are examples of all three of these new instructions in the above code. Implement these
first – they will help you debug to real Y86 instructions.


Starting the Assignment
The starting handout, available on blackboard, contains several files:
 simulate.c – this is the file you will be editing
 simulate_main.c – reads the input into the data structure you will be using.
 Makefile - creates executable ‘simulate’
 Two examples to initially try: test1.m and test2.m – Try running ‘simulate test1.m’.
Iniitally doesn’t do much but eventually will execute the Y86 program test1.Y. There is
also a test2.Y. Use the give tools to create other test files.
Implementation Notes
 You will need to use 64-bit (long int) for your registers and other 64-bit values. To print
a 64-bit value in hex, use %lx (rather than %x).
 You must use char type (single byte) and bit-level operators to do your work. Use
masks and shifting to get to the parts of the byte you need to use at a given time. Be
careful about shifting bytes since arithmetic right shifts are used in C. Casting can also
cause problems since sign extension is going to be used.
 There are a couple of tools you can use to help create other test files. While copying
these executables is an option, it is a better idea to just run them from the 367 account.
The tools were just written and any bug fixes or upgrades will be available to you this
way. There is little or no error checking in these tools so you may get unexpected
results from bad input.
o /home/cs367/Y86/assemble – an assembler for the version of Y86 described here.
‘assemble test.Y’ will create a text file like that shown above with starting PC = 0.
If you add an additional decimal parameter ‘assemble test.Y 256’ then the given
number will be used as the starting program counter. The tool currently only
produces to file ‘out.m’ but this may be changed in the near future to let you
output to a file you specify.
o /home/cs367/Y86/disassemble – this will reverse the first step and take you back
to Y86.
Submitting & Grading
Submit simulate.c on blackboard. It must still build with the rest of the code using the
given Makefile.
Your grade will be determined as follows:
 80 points – Correctness – to be completely correct, you have to both generate the correct
addresses and data AND get the information to do the generation from the correct
location.
 20 points – use of C, comments, bit-level operators…
