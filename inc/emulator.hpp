#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include "assembler.hpp"
#include <iostream>
#include <vector>
using namespace std;


#include <string>
#include <string.h>
#include <fstream>
#include <unordered_map>
#include <map>
#include <tuple>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


class Emulator {
private:
  string inputFile;
  int fd;
  char* mem;
  
  bool handlerSet = false;
  
  //uint pc; this is actually r15
  //uint sp; this is actually r14
  
  vector<uint> reg  = vector<uint>(16);
  vector<uint> ctrlReg = vector<uint>(3);

public:
  void setInputFile(char* in);
  void startEmulating();
  
  void initializeEmulator();
  void doInterruptStuff();
  void irregularOp();
  void push(uint value);
  uint readMem(uint location);
  void writeMem(uint location, uint value);
  //
};




#endif