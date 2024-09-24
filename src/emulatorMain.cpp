#include "../inc/emulator.hpp"
#include <string>
#include <string.h>


int main(int argc, char** argv){
  if (argc==1){
    cout<<"Input file name missing"<<endl;
    exit(-1);
  }
  if (argc>2){
    cout<<"Too many arguments"<<endl;
    exit(-1);
  }
  if (strcmp(argv[0], "./emulator")==0){
    // emulator
    Emulator emulator = Emulator();
    emulator.setInputFile(argv[1]);
    emulator.startEmulating();
  }



  return 0;
}


// Expected output for lvl A

/*
-----------------------------------------------------------------
Emulated processor executed halt instruction
Emulated processor state:
 r0=0x00000000     r1=0x0000abcd     r2=0x00000002     r3=0x00000003
 r4=0x00000004     r5=0x00000005     r6=0x00000006     r7=0x00000000
 r8=0x00000000     r9=0x00000000    r10=0x00000000    r11=0x00000000
r12=0x00000000    r13=0x00000000    r14=0xfffffed6    r15=0x400000c4
*/