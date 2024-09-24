#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "symbolTableEntry.hpp"
#include "sectionTable.hpp"
#include "relocTableEntry.hpp"
#include "literalTable.hpp"

using namespace std;


// instructions with MMMM just use or 
  // for the mode i guess
enum OpCode {
  C_HALT = 0b00000000,
  C_INT  = 0b00010000,
  C_CALL = 0b00100000,
  C_JMP = 0b00110000,
  C_XCHG = 0b01000000,
  C_ARITH = 0b01010000,
  C_LOG = 0b01100000,
  C_SHIFT = 0b01110000,
  C_ST = 0b10000000,
  C_LD = 0b10010000 
};

enum Reg {
  R0 = 0b0000,
  R1,
  R2,
  R3,
  R4,
  R5,
  R6,
  R7,
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15,
  SP = R14,
  PC = R15,
  STATUS = R0,
  HANDLER = R1,
  CAUSE = R2
};



class Assembler {
  private:
    int locationCounter = 0;
    string currentSection = "";
    string outFileName;
    ofstream outfile;
    vector<SymbolTableEntry> symbolTable;
    vector<SectionTableEntry> sectionTable;
    // key is section name; value is literal table for given section
    unordered_map<string, vector<LiteralTableEntry>> literalTables;
    // key is section name; value is relocation table for given section
    unordered_map<string, vector<RelocTableEntry>> relocTables;
    // key is section name; value is machine code for given section
    unordered_map<string, vector<char>> machineCodes;

  public:
    void firstPass (int lineNum);
    void finishFirstPass ();
    void secondPass();
    void finishSecondPass ();

    void setOutputFile(string out);
    void writeIntoBinaryFile();

    void addSymbolIntoTable (int value, Type type, Bind bind, int sectionIndex, string name);
    int  getSectionIndex (string name);
    int addSectionIntoTable (int locationCounter, string currentSection, string name, int base);
    void addLiteralIntoTable (int value);
    int addSymbolIntoLiteralTable (int symbolIndex);
    // also returns index of symbol 
    int checkIfSymbolInTable (string symbol);
    void addRelocIntoTable(string section, int relocPlace, RelocType rType, int symbolIndex);
    // up to FOUR bytes (an int); cba to make it better
    void addIntoMachineCode (int value, int numOfBytes=4);
    void addIntoSpecificMachineCode (int value, string section,  int numOfBytes  = 4);
    LiteralTableEntry getLiteralTableEntry (int value);
    LiteralTableEntry getLiteralTableEntryLabel (string label);
};






#endif