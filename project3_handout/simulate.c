#include <stdio.h>
#include <stdlib.h>


int Pop(int), Push(int), Call(int), Ret(int), Jump(int);
int IRmov(int), RRmov(int), RMmov(int), MRmov(int);
int OPx(int);
int printall(int), printreg(int),printmem(int);

char SF = 0x00;
char ZF = 0x00;

extern char memory[] ;
extern int PC ;

long int regs[16];

char *regname[15] = {"%rax","%rcx","%rdx","%rbx","%rsp","%rbp",
                "%rsi","%rdi","%r8","%r9","%r10","%r11","%r12","%r13","%r14"};



void
execute(int PC) {
   int done = 0;
   char byte;
   char opcode ;

   while (!done) {
      byte = memory[PC];
      opcode = (byte >> 4)&0xf;
      switch (opcode) {
	 case 0: printf("Halting at instruction 0x%x\n",PC);
          printf("Print MEM val: %hhx\n", memory[PC]);
		done = 1; break;  //halt
	 case 1: PC++; break;   // nop
	 case 2: PC = RRmov(PC);
		 break;
	 case 3: PC = IRmov(PC);
		 break;
	 case 4: PC = RMmov(PC);
		 break;
	 case 5: PC = MRmov(PC);
		 break;
	 case 6: PC = OPx(PC);
		 break;
	 case 7:  PC = Jump(PC);
		  break;
	 case 8: PC = Call(PC);
		 break;
	 case 9: PC = Ret(PC);
		 break;
	 case 10: PC = Push(PC);
		  break;
	 case 11: PC = Pop(PC);
		  break;
    case 12: PC = printall(PC);
		   break;
	 case 13: PC = printreg(PC);
		  break;
	 case 14: PC = printmem(PC);
		  break;
	 default:
	 	printf("# unknown op at PC=0x%x\n",PC); PC++;
      }
   }
}


int
printreg(int PC) {
  // printf("PC: %d\n", PC);
  int reader = PC+1;
  long int value = 0x00;
  // printf("It does not show up\n");
  for (;reader>PC;reader--){
      // value<<=8;
      value |= ((memory[reader])&0xf0);
  }
    value >>=4;
  // printf("val: %lx\n", value);

  printf(">> %s : 0x%lx\n", regname[value], regs[value]);
  // printf("PC: %d\n", PC);
   return PC+2;
}

int
printall (int PC) {

  for (int i = 0; i < 15; i++){
    printf(">>%s: %lx |\n", regname[i], regs[i]);
  }
   return PC+1;
}

int
printmem(int PC) {
  int reader = PC + 9;
  long int displacement = 0x00;
  for(;reader > PC + 1; reader--){
    displacement <<= 8;
    displacement |= ((memory[reader])&0xff);
  }
  // printf("Print MEM val: %lx\n", displacement);
  printf(">> %s : 0x%lx\n",regname[(memory[reader]&0xf0)>>4], regs[(memory[reader]&0xf0)>>4]+displacement);
  // regs[(memory[reader]&0xf0)>>4];
   return PC + 10;
}

int
IRmov(int PC) {

  // printf("PC: %d\n", PC);
  long int value = 0x00;
  int reader = PC + 9;
  // printf("Reader PreFor: %d\n", reader);
  for (;reader>PC+1;reader--){
    // printf(">>Reader: %d \t", reader);
    // printf(">>|Mem: %lx \t", (long int)memory[reader]);
    value<<=8;
    value |= ((memory[reader])&0xff);

    // printf("val: %lx\t", value);

    // value |= ((memory[reader])&0xf);
    // printf("val: %lx\n", value);

  }
  // value >>=8;
  // printf("Reader EndFor: %d\n", reader);
  // printf(">>Mem: %lx \n", (long int)memory[reader]&0x0f);
  // printf(">>Register: %s ", regname[(long int)memory[reader]&0x0f]);
  regs[(memory[reader]&0x0f)] = value;

  // printf("Value: %lx\n", value);
   return PC+10;
}

int
RRmov(int PC) {

  int reader = PC;

  // printf("Start\n");

  long int operand = 0x00;
  operand |= (memory[reader] & 0xf);
  reader++;

  // if(operand == 0)

   return PC+2;
}

int
RMmov(int PC) {

  long int displacement = 0x00;
  int reader = PC + 9;

  for(;reader>PC+1;reader--){
    displacement<<=8;
    displacement |= ((memory[reader])&0xff);
  }

  long int source = ((memory[reader])&0xf0)>>4;
  long int dest = (memory[reader]&0x0f);

  memory[regs[dest] + displacement]= (char)regs[source];
  //Read 10 bytes
  //Register to Memory move
  //Write to memory
  // PC ^^ 10
   return PC+10;
}

int
MRmov(int PC) {

  long int displacement = 0x00;
  int reader = PC+ 9;

  for(;reader>PC+1;reader--){
    displacement<<=8;
    displacement |= ((memory[reader])&0xff);
  }
  // printf("MR Displacement Val: %lx\n", displacement);
  long int source = (memory[reader])&0x0f;
  long int dest = (memory[reader]&0xf0)>>4;

  regs[dest] = regs[source]+displacement;
   return PC+10;
}

int
Jump(int PC ) {

  int reader = PC;
  // printf("Start\n");

  //Set the operand for the conditional JUMP functions
  long int operand = 0x00;
  operand |= (memory[reader] & 0xf);
  // reader++;

  //PC + 1
  // //Get the source and dest registert
  // long int source = ((memory[reader])&0xf0)>>4;
  // long int dest = (memory[reader]&0x0f);

  //Set reader to the end of the destination bytes
  reader = PC + 8;

  //Get the destination for conditonal jumps
  long int destination = 0x00;
  for(;reader>PC+1;reader--){
    destination<<=8;
    destination |= ((memory[reader])&0xff);
  }


  if (operand == 0){ //jump always
    PC = (int)memory[destination];
  }
  if (operand == 1){ //jump LE
    if (ZF == 0x01 || SF == 0x01)
      PC = (int)memory[destination];
  }
  if (operand == 2){ //jump L
    if (SF == 0x01)
      PC = (int)memory[destination];
  }
  if (operand == 3){ //jump E
    if (ZF == 0x01)
      PC = (int)memory[destination];
  }
  if (operand == 4){ //jump NE
    if (ZF == 0x00)
      PC = (int)memory[destination];
  }
  if (operand == 5){ //jump GE
PC = (int)memory[destination];
  }
  if (operand == 6){ //jump G
    if(SF == 0x00 && ZF == 0x00)
      PC = (int)memory[destination];
  }
	return PC;
}

int
Call(int PC) {
   return PC + 9;
}

int
Ret(int PC) {
    return PC+1;
}

int
Push(int PC) {
  return PC+2;
}

int
Pop(int PC) {
  //Read 2 bytes
  //Read Stack Pointer
  //Increment Stack Pointer by 8
  //Read from Old Stack Pointer
  //Update Stack Pointer/ write result into register
  //PC ^^2
  return PC+2;
}


int
OPx(int PC) {
  int reader = PC;

  // printf("Start\n");

  long int operand = 0x00;
  operand |= (memory[reader] & 0xf);
  // long int func = PC&0x;
  reader++;

  //PC + 1;
  long int source = ((memory[reader])&0xf0)>>4;
  long int dest = (memory[reader]&0x0f);

  if (operand == 0){ //Add D = S + D
    regs[dest] = regs[source]+regs[dest];

    if(regs[dest] == 0)
      ZF = 0x01;
    else
      ZF = 0x00;
    if (regs[dest] < 0)
      SF = 0x01;
    else
      SF = 0x00;
  }
  else if (operand == 1){ //Subtract D = D-S
    regs[dest] = regs[dest] - regs[source];
    if(regs[dest] == 0)
      ZF = 0x01;
    else
      ZF = 0x00;
    if (regs[dest] < 0)
      SF = 0x01;
    else
      SF = 0x00;
  }
  else if (operand == 2){ //And D = S&D
    regs[dest] = regs[source]&regs[dest];
    if(regs[dest] == 0)
      ZF = 0x01;
    else
      ZF = 0x00;
    if (regs[dest] < 0)
      SF = 0x01;
    else
      SF = 0x00;
  }
  else if (operand == 3){ //xorq D = S^D
    regs[dest] = regs[source]^regs[dest];
    if(regs[dest] == 0)
      ZF = 0x01;
    else
      ZF = 0x00;
    if (regs[dest] < 0)
      SF = 0x01;
    else
      SF = 0x00;
  }
  // printf(">>>>val: %lx\n", operand);
  // printf(">> S: %s | D: %s\n", regname[source], regname[dest]);
  // printf("end\n");
   return PC+2;
}
