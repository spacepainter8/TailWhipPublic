#include "../inc/assembler.hpp"
#include "../inc/parserHelper.hpp"
#include <math.h>

#include "assembler.hpp"
#include <iostream>
using namespace std;


void Assembler::firstPass(int lineNum){
  Line l;
  Line nextL;
  for (int i=0;i<parserHelper::code.size();i++){
      if (parserHelper::code[i].lineNum==lineNum){
        l = parserHelper::code[i];
        if (i+1<parserHelper::code.size() && parserHelper::code[i+1].lineNum==lineNum) {nextL = parserHelper::code[i+1];}
        break;
      }
  }

  if (l.name==""){
    outfile<<"LINE NOT IN MY CODE VECTOR!!"<<endl;
    exit(-1);
  }

  // if nextL is a label -> process it.
  if (nextL.name!="") {
    //must be in a section
    if (this->currentSection==""){
      // label out of section
      // error
      outfile<<"Line "<<nextL.lineNum<<": Label can't be out of a section"<<endl;
      exit(-1);
    }
    // get sectionIndex first
    int index = this->getSectionIndex(this->currentSection);
    // add symbol into table
    this->addSymbolIntoTable(this->locationCounter, Type::NOTYP, Bind::LOC, index, nextL.name);
    // in case it's like label:command

    // if (nextL.name!="") l = nextL;
    // else return;
  }

  // first switch choose command type
  // second switch choose exact command
  // doesn't really matter, just maybe faster idk

  switch (l.type){
    case Types::label: {
      //must be in a section
      if (this->currentSection==""){
        // label out of section
        // error
        outfile<<"Line "<<l.lineNum<<": Label can't be out of a section"<<endl;
        exit(-1);
     }
      // get sectionIndex first
      int index = this->getSectionIndex(this->currentSection);
      // add symbol into table
      this->addSymbolIntoTable(this->locationCounter, Type::NOTYP, Bind::LOC, index, l.name);
      // in case it's like label:command

      // if (nextL.name!="") l = nextL;
      // else return;
      break;
    }
    case Types::directive: {

      if (l.name=="global"){
        for (auto arg: l.args){
          this->addSymbolIntoTable(0, Type::NOTYP, Bind::GLOB, -1, arg.argS);
        }
      }

      else if (l.name=="section"){

        int index = this->addSectionIntoTable(this->locationCounter, this->currentSection, l.args[0].argS, 0);
        this->locationCounter = 0;
        this->currentSection = l.args[0].argS;
        this->addSymbolIntoTable(0, Type::SCTN, Bind::GLOB, index, this->currentSection);
        
      }

      else if (l.name=="extern"){
        for (auto arg:l.args){
          this->addSymbolIntoTable(0, Type::NOTYP, Bind::GLOB, -2, arg.argS);
        }
      }

      else if (l.name=="word"){
        int count = 0;
        for (auto arg:l.args) count++;
        this->locationCounter+=4*count;
      }

      else if (l.name=="skip"){
        this->locationCounter+=l.args[0].argI;
      }

      else if (l.name=="end"){
        //outfile<<endl<<this->locationCounter<<endl<<endl;

        this->finishFirstPass();

        // outfile<<"SYMBOL TABLE"<<endl;
        // for(auto symbol:this->symbolTable){
        //   outfile<<symbol;
        // }

        // outfile<<endl<<endl;

        // outfile<<"SECTION TABLE"<<endl;
        // for(auto section:this->sectionTable){
        //   outfile<<section;
        // }

        // outfile<<endl<<endl;

        // outfile<<"LITERAL TABLE"<<endl;
        // for(auto literal:this->literalTable){
        //   outfile<<literal;
        // }

        return; 
      }

      else {
        // shouldn't come here but aight
        outfile<<"Line "<<l.lineNum<<": Unsupported directive!!"<<endl;
        exit(-1);
      }

      break;
    }

    case Types::command: {
      
      if (l.name=="halt"){
        this->locationCounter+=4;
      }
      
      else if (l.name=="int"){
        this->locationCounter+=4;
      }

      else if (l.name=="iret"){
        this->locationCounter+=8;
      }

      else if (l.name=="call"){
        this->locationCounter+=4;
        // if operand is literal, possibly need to add to literal table
        // if it's a label, don't do anything -> we don't know anything for now
        // NISAM SIGURNA ZA RACUNICU OKO LITERALA PITATI NEKOGA
        int literal;
        if (l.args[0].argS=="") {
          literal = l.args[0].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
          // signed literal can't fit in 12 bits, need to add to literal table
            this->addLiteralIntoTable(literal);
          }  
        }
        
      }

      else if (l.name=="ret"){
        this->locationCounter+=4;
      }

      else if (l.name=="jmp"){
        this->locationCounter+=4;
        int literal;
        if (l.args[0].argS=="") {
          literal = l.args[0].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
          // signed literal can't fit in 12 bits, need to add to literal table
            this->addLiteralIntoTable(literal);
          }  
        }
      }

      else if (l.name=="beq"){
        this->locationCounter+=4;
        int literal;
        if (l.args[2].argS=="") {
          literal = l.args[2].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
          // signed literal can't fit in 12 bits, need to add to literal table
            this->addLiteralIntoTable(literal);
          }  
        }
      }

      else if (l.name=="bne"){
        this->locationCounter+=4;
        int literal;
        if (l.args[2].argS=="") {
          literal = l.args[2].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
          // signed literal can't fit in 12 bits, need to add to literal table
            this->addLiteralIntoTable(literal);
          }  
        }
      }

      else if (l.name=="bgt"){
        this->locationCounter+=4;
        int literal;
        if (l.args[2].argS=="") {
          literal = l.args[2].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
          // signed literal can't fit in 12 bits, need to add to literal table
            this->addLiteralIntoTable(literal);
          }  
        }
      }

      else if (l.name=="push"){
        this->locationCounter+=4;
      }

      else if (l.name=="pop"){
        this->locationCounter+=4;
      }

      else if (l.name=="xchg"){
        this->locationCounter+=4;
      }

      else if (l.name=="add" || l.name=="sub" || l.name=="mul" || l.name=="div"){
        this->locationCounter+=4;
      }

      else if (l.name=="not" || l.name=="and" || l.name=="or" || l.name=="xor"){
        this->locationCounter+=4;
      }

      else if (l.name=="shl" || l.name=="shr"){
        this->locationCounter+=4;
      }

      else if (l.name=="ld"){
        if (l.args[0].argS=="$"){
          if (l.args[1].argS==""){
            // $literal
            int literal = l.args[1].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
              // signed literal can't fit in 12 bits, need to add to literal table
              this->addLiteralIntoTable(literal);
              this->locationCounter+=4;
            }
            else this->locationCounter+=4;
          }
          else {
            // $symbol
            this->locationCounter+=4;
          }
        }
        else if (l.args[0].argS=="%"){
          // reg
          this->locationCounter+=4;
        }
        else if (l.args[0].argS=="[]"){
          if (l.args.size()==3){
            // [reg]
            this->locationCounter+=4;
          }
          else {
            if (l.args[1].argS==""){
              // [reg+literal]
              int literal = l.args[2].argI;
              if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
                // literal can't be bigger than 12b
                outfile<<"Line "<<l.lineNum<<": [reg+literal] can't deal with a literal wider than 12b!!"<<endl;
                exit(-1);
              }
              this->locationCounter+=4;
            }
            else {
              // [reg+symbol]
              outfile<<"Line "<<l.lineNum<<": Final value of symbol unknown at assembling!!"<<endl;
              exit(-1);
            }
          }
        }
        else {
          if (l.args[0].argS==""){
            // literal
            int literal = l.args[0].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
              // signed literal can't fit into 12 bits
              this->addLiteralIntoTable(literal);
              this->locationCounter+=8;
            }
            else {
              this->locationCounter+=4;
            }
          }
          else {
            // symbol
            this->locationCounter+=8;
          }
        }
      }

      else if (l.name=="st"){
        if (l.args[1].argS=="$"){
          outfile<<"Line "<<l.lineNum<<": Storing into a literal or a symbol doesn't make much sense!!"<<endl;
          exit(-1);
        }
        else if (l.args[1].argS=="%"){
          // reg
          this->locationCounter+=4;
        }
        else if (l.args[1].argS=="[]"){
          if (l.args.size()==3){
            // [reg]
            this->locationCounter+=4;
          }
          else if (l.args[2].argS==""){
            // [reg+literal]
            int literal = l.args[3].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
                outfile<<"Line "<<l.lineNum<<": [reg+literal] literal must fit in 12b!!"<<endl;
                exit(-1);
            }
            this->locationCounter+=4;
          }
          else {
            // [reg+symbol]
            outfile<<"Line "<<l.lineNum<<": Final value of symbol unknown at assembling!!"<<endl;
            exit(-1);
          }
        }
        else {
          if (l.args[1].argS==""){
            // literal
            int literal = l.args[1].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
              this->addLiteralIntoTable(literal);
            }
            this->locationCounter+=4;
          }
          else {
            // symbol
            this->locationCounter+=4;
          }
        }
      }

      else if (l.name=="csrrd" || l.name=="csrwr"){
        this->locationCounter+=4;
      }

      else {
        outfile<<"Line "<<l.lineNum<<": Unsupported command!!"<<endl;
        exit(-1);
      }


      break;
    }
    
    default: {
      // error
      // shouldn't really ever come here but aight
      outfile<<"Line "<<l.lineNum<<": Not a label, directive or a command??"<<endl;
      exit(-1);


    }
  }
}

void Assembler::finishFirstPass(){
 

 // any symbol declared as global, but not defined??
  for (auto symbol:this->symbolTable){
    if (symbol.getSectionIndex()==-1){
      outfile<<"Symbol "<<symbol.getName()<<" declared as global, but wasn't defined!!"<<endl;
      exit(-1);
    }
  }

  // finish up current section entry in literalTables
  for (auto& entry:this->literalTables[this->currentSection]){
    entry.setLocation(this->locationCounter);
    this->locationCounter+=4;
  }


 // finish up current section entry in section table (set length)
  for (auto& section:this->sectionTable){
    if (section.getName()==this->currentSection){
      section.setLength(this->locationCounter);
      break;
    }
  }

  this->locationCounter=0;
  this->currentSection="";

}

void Assembler::secondPass(){
  for (Line l:parserHelper::code){
    if (l.type==Types::label) continue;
    if (l.type==Types::directive){
      if (l.name=="global" || l.name=="extern") continue;
      else if (l.name=="section"){
        

        this->locationCounter=0;
        this->currentSection = l.args[0].argS;
        vector<RelocTableEntry> rte;
        this->relocTables[this->currentSection] = rte;
        vector<char> code;
        this->machineCodes[this->currentSection] = code;
        
        
      }
      else if (l.name=="word"){
        // if symbol, check if in table
          // if yes leave reloc entry
          // if not abort
        // if literal, write into code
        for (auto arg:l.args){
          if (arg.argS==""){
            // literal
            int literal = arg.argI;
            this->addIntoMachineCode(literal, 4);
          }
          else {
            // symbol
            int symbolIndex = this->checkIfSymbolInTable(arg.argS);
            this->addIntoMachineCode(0b00000000, 4);
            this->addRelocIntoTable(this->currentSection, this->locationCounter-4, RelocType::R_32, symbolIndex);

          }
        }

      }
      
      else if (l.name=="skip"){
        int num = l.args[0].argI;
        this->locationCounter+=num;
        for (int i=0;i<num;i++){
          this->machineCodes[this->currentSection].push_back(0b00000000);
        }
      }

      else if (l.name=="end"){
        // finish second pass
        // write into object files
        
        this->finishSecondPass();

        ulong sum = 0;
        for (auto section:this->sectionTable){
          sum+=section.getLength();
          if (section.getLength()>pow(2,12)){
            cout<<"Section can't be longer than 2^12!!"<<endl;
            exit(-1);
          }
        }

        if (sum>pow(2,32)){
          cout<<"Sum of sections bigger than 2^32!!"<<endl;
          exit(-1);
        }
         
        outfile<<"SYMBOL TABLE"<<endl;
        outfile<<"INDEX"<<"\t"<<"VALUE"<<"\t"<<"TYPE"<<"\t"<<"BIND"<<"\t"<<"SECTION_INDEX"<<"\t"<<"NAME"<<endl;
        for(auto symbol:this->symbolTable){
          outfile<<symbol;
        }

        outfile<<endl<<endl;

        outfile<<"SECTION TABLE"<<endl;
        outfile<<"INDEX"<<"\t"<<"NAME"<<"\t"<<"LENGTH"<<endl;
        for(auto section:this->sectionTable){
          outfile<<section;
        }

        outfile<<endl<<endl;

        outfile<<"LITERAL TABLES"<<endl;
       for (auto lTable:this->literalTables){
        outfile<<endl<<lTable.first<<endl;
        outfile<<"VALUE"<<"\t"<<"LOCATION"<<"\t"<<"NAME"<<endl;
        for (auto entry:lTable.second){
            outfile<<entry<<endl;
        }
       }

       outfile<<endl<<"RELOC TABLES"<<endl;
       for (auto lTable:this->relocTables){
        outfile<<endl<<lTable.first<<endl;
        outfile<<"OFFSET"<<"\t"<<"SYMBOL_INDEX"<<"\t"<<"ADDEND"<<"\t"<<"RELOC_TYPE"<<endl;
        for (auto entry:lTable.second){
            outfile<<entry<<endl;
        }
       }

       outfile<<endl<<"MACHINE CODES"<<endl;
       
       for (auto machineTable:this->machineCodes){
        int location=0;
        outfile<<machineTable.first<<endl;
        //outfile<<location<<": ";
        int numOfChars = machineTable.second.size();
        int bigcnt = 0;
        while (numOfChars>=10){
          numOfChars = numOfChars/10;
          bigcnt++;
        }
        numOfChars = machineTable.second.size();
        bigcnt++;
        for(char machine:machineTable.second){
          char hex[9];
          snprintf(hex, 9, "%02X", machine);
          if (machine<0){
            if (location%8==0) {
              int smallCnt = 0;
              int locationTemp = location;
              while(locationTemp>=10){
                locationTemp = locationTemp/10;
                smallCnt++;
              }
              smallCnt++;
              string str = "00000000";
              sprintf(&(str[0]), "%08X", location);
              outfile<<endl<<str<<": ";
              int cnt = bigcnt - smallCnt;
              //while (cnt>0) {outfile<<" ";cnt--;}
            }
            outfile<<hex[6]<<hex[7]<<" ";
            location++;
            if (location%4==0) {outfile<<"\t";}
          }
          else {
            if (location%8==0) {
              int smallCnt = 0;
              int locationTemp = location;
              while(locationTemp>=10){
                locationTemp = locationTemp/10;
                smallCnt++;
              }
              smallCnt++;
              string str = "00000000";
              sprintf(&(str[0]), "%08X", location);
              outfile<<endl<<str<<": ";
              int cnt = bigcnt - smallCnt;
              //while (cnt>0) {outfile<<" ";cnt--;}
            }
            outfile<<hex[0]<<hex[1]<<" ";
            location++;
            if (location%4==0) outfile<<"\t";

            
          }
        }
        outfile<<endl;
       
        }
         outfile.close();
         this->writeIntoBinaryFile();
       }

       else {
          outfile<<"Line "<<l.lineNum<<": Unsupported directive!!"<<endl;
          exit(-1);
        }

        

      }

      

    

    else if (l.type==Types::command){
      
      if (l.name=="halt"){
        
        this->addIntoMachineCode(OpCode::C_HALT, 4);
       
      }

      else if (l.name=="int"){
        int code = OpCode::C_INT<<24;
        this->addIntoMachineCode(code, 4);
      }

      else if (l.name=="iret"){
        // pop status
        int code = ((OpCode::C_LD | 0b0110) << 24) | (Reg::STATUS<<20) | (Reg::SP<<16) | (0b0100);
        this->addIntoMachineCode(code);
        // pop pc and move sp up by 8
        code = ((OpCode::C_LD | 0b0011) << 24) | (Reg::PC << 20) | (Reg::SP << 16) | (0b1000);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="call"){
        if (l.args[0].argS==""){
          // literal
         int literal = l.args[0].argI; 
         if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
          // literal doesn't fit in 12 b
          int poolAddress = this->getLiteralTableEntry(literal).getLocation();
          int displacement  = poolAddress - (this->locationCounter+4);
          if (displacement>(pow(2,12)-1)){
            outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
            exit(-1);
          }
          int code = ((OpCode::C_CALL | 0b0001)<<24) | (Reg::PC<<20) | (displacement & 0x00000fff);
          this->addIntoMachineCode(code);

         }
         else {
          // literal fits in 12b
          int code = ((OpCode::C_CALL | 0b0000)<<24) | (literal & 0x00000fff);
          this->addIntoMachineCode(code);
         }

        }
        else {
          // symbol
          string symbol = l.args[0].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          int currentSectionIndex = this->getSectionIndex(this->currentSection);
          if (this->symbolTable[symbolIndex].getSectionIndex()==currentSectionIndex){
            // symbol native to current section
            int displacement = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            
            int code = ((OpCode::C_CALL)<<24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // symbol not native to current section and/or .extern
            // add into literal table
            // leave relocation in relocTable
            this->addSymbolIntoLiteralTable(symbolIndex);
            LiteralTableEntry lte = this->getLiteralTableEntryLabel(symbol);
            this->addRelocIntoTable(this->currentSection, lte.getLocation(), RelocType::R_32, symbolIndex);
            int displacement = lte.getLocation()-(this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_CALL | 0b0001)<<24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
      }

      else if (l.name=="ret"){
        int code = ((OpCode::C_LD | 0b0011)<<24) | (Reg::PC<<20) | (Reg::SP<<16) | (0b0100);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="jmp"){
        if (l.args[0].argS==""){
          // literal
          int literal = l.args[0].argI; 
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
            // literal can't fit in 12 bits
            LiteralTableEntry lte = this->getLiteralTableEntry(literal);
            int displacement = lte.getLocation() - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code  = ((OpCode::C_JMP | 0b1000)<<24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // literal can fit in 12 bits
            int code = (OpCode::C_JMP << 24) | (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
        else {
          // symbol
          string symbol = l.args[0].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
            // symbol native to current section
            int displacement = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = (OpCode::C_JMP << 24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // symbol not native to current section and/or .extern
            // literal table
            // relocation
            int location = this->addSymbolIntoLiteralTable(symbolIndex);
            this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
            int displacement = location - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b1000) << 24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
      }

      else if (l.name=="beq"){
        if (l.args[2].argS==""){
          // literal
          int literal = l.args[2].argI; 
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
            // literal can't fit in 12 bits
            LiteralTableEntry lte = this->getLiteralTableEntry(literal);
            int displacement = lte.getLocation() - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b1001) << 24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // literal can fit in 12 bits
            int code = ((OpCode::C_JMP | 0b0001)<<24) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
        else {
          // symbol
          string symbol = l.args[2].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
            // symbol native to current section
            int displacement = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b0001)<<24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // symbol not native to current section and/or .extern
            // literal table
            // relocation
            int location = this->addSymbolIntoLiteralTable(symbolIndex);
            this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
            int displacement = location - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            //int code = ((OpCode::C_JMP | 0b1000) << 24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            int code = ((OpCode::C_JMP | 0b1001)<<24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
      }

      else if (l.name=="bne"){
        if (l.args[2].argS==""){
          // literal
          int literal = l.args[2].argI; 
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
            // literal can't fit in 12 bits
            LiteralTableEntry lte = this->getLiteralTableEntry(literal);
            int displacement = lte.getLocation() - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b1010) << 24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // literal can fit in 12 bits
            int code = ((OpCode::C_JMP | 0b0010)<<24) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
        else {
          // symbol
          string symbol = l.args[2].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
            // symbol native to current section
            int displacement = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b0010)<<24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // symbol not native to current section and/or .extern
            // literal table
            // relocation
            int location = this->addSymbolIntoLiteralTable(symbolIndex);
            this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
            int displacement = location - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            //int code = ((OpCode::C_JMP | 0b1000) << 24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            int code = ((OpCode::C_JMP | 0b1010)<<24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
      }

      else if (l.name=="bgt"){
        if (l.args[2].argS==""){
          // literal
          int literal = l.args[2].argI; 
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
            // literal can't fit in 12 bits
            LiteralTableEntry lte = this->getLiteralTableEntry(literal);
            int displacement = lte.getLocation() - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b1011) << 24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // literal can fit in 12 bits
            int code = ((OpCode::C_JMP | 0b0011)<<24) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
        else {
          // symbol
          string symbol = l.args[2].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
            // symbol native to current section
            int displacement = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_JMP | 0b0011)<<24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // symbol not native to current section and/or .extern
            // literal table
            // relocation
            int location = this->addSymbolIntoLiteralTable(symbolIndex);
            this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
            int displacement = location - (this->locationCounter+4);
            if (displacement>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            //int code = ((OpCode::C_JMP | 0b1000) << 24) | (Reg::PC<<20) | (displacement & 0x00000fff);
            int code = ((OpCode::C_JMP | 0b1011)<<24) | (Reg::PC<<20) | (l.args[0].argI<<16) | (l.args[1].argI<<12) | (displacement & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
      }

      else if (l.name=="push"){
        int disp = -4;
        int code = ((OpCode::C_ST | 0b0001)<<24) | (Reg::SP<<20) | (l.args[0].argI<<12) | (disp & 0x00000fff);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="pop"){
        int code = ((OpCode::C_LD | 0b0011)<<24) | (l.args[0].argI<<20) | (Reg::SP<<16) | (0b0100);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="xchg"){
        int code = (OpCode::C_XCHG<<24) | (l.args[0].argI<<16) | (l.args[1].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="add"){
        int code = ((OpCode::C_ARITH | 0b0000)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="sub"){
        int code = ((OpCode::C_ARITH | 0b0001)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="mul"){
        int code = ((OpCode::C_ARITH | 0b0010)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="div"){
        int code = ((OpCode::C_ARITH | 0b0011)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="not"){
        int code = (OpCode::C_LOG << 24) | (l.args[0].argI<<20) | (l.args[0].argI<<16);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="and"){
        int code = ((OpCode::C_LOG | 0b0001)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="or"){
        int code = ((OpCode::C_LOG | 0b0010)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="xor"){
        int code = ((OpCode::C_LOG | 0b0011)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="shl"){
        int code = ((OpCode::C_SHIFT | 0b0000)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="shr"){
        int code = ((OpCode::C_SHIFT | 0b0001)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16) | (l.args[0].argI<<12);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="csrrd"){
        int csr = -1;
        if (l.args[0].argS=="status") csr = 0;
        else if (l.args[0].argS=="handler") csr = 1;
        else if (l.args[0].argS=="cause") csr = 2;
        int code = ((OpCode::C_LD | 0b0000)<<24) | (l.args[1].argI<<20) | (csr<<16);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="csrwr"){
        int csr = -1;
        if (l.args[1].argS=="status") csr = 0;
        else if (l.args[1].argS=="handler") csr = 1;
        else if (l.args[1].argS=="cause") csr = 2;
        int code = ((OpCode::C_LD | 0b0100)<<24) | (csr<<20) | (l.args[0].argI<<16);
        this->addIntoMachineCode(code);
      }

      else if (l.name=="ld"){
        if (l.args[0].argS=="$"){
          if (l.args[1].argS==""){
            // $literal
            int literal = l.args[1].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
              // literal can't fit in 12 bits
              LiteralTableEntry lte = this->getLiteralTableEntry(literal);
              int disp = lte.getLocation() - (this->locationCounter+4);
              if (disp>(pow(2,12)-1)){
                outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
                exit(-1);
              }
              int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[2].argI<<20) | (Reg::PC<<16) | (disp & 0x00000fff);
              this->addIntoMachineCode(code);
            }
            else {
              // literal can fit in 12 bits
              int code = ((OpCode::C_LD | 0b0001) << 24) | (l.args[2].argI << 20) | (literal & 0x00000fff);
              this->addIntoMachineCode(code);
            }
          }
          else {
            // $symbol
            string symbol = l.args[1].argS;
            int symbolIndex = this->checkIfSymbolInTable(symbol);
            if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
              // label native to this section
              int disp = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
              if (disp>(pow(2,12)-1)){
                outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
                exit(-1);
              }
              int code = ((OpCode::C_LD | 0b0001)<<24) | (l.args[2].argI<<20) | (Reg::PC<<16) | (disp & 0x00000fff);
              this->addIntoMachineCode(code);
            }
            else {
              // label not native to this section and/or .extern
              int location = this->addSymbolIntoLiteralTable(symbolIndex);
              this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
              int disp = location - (this->locationCounter+4);
              if (disp>(pow(2,12)-1)){
                outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
                exit(-1);
              }
              int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[2].argI<<20) | (Reg::PC<<16) | (disp & 0x00000fff);
              this->addIntoMachineCode(code);
            }
          }
        }
        else if (l.args[0].argS=="%"){
          // 1 -> reg1, 2 -> , reg2 je odredisni
          int code = ((OpCode::C_LD | 0b0001)<<24) | (l.args[2].argI<<20) | (l.args[1].argI<<16);
          this->addIntoMachineCode(code);
        }
        else if (l.args[0].argS=="[]"){
          if (l.args.size()==3){
            // [reg]
            // 1 -> reg1, 2 -> reg2 (odredisni)
            int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[2].argI<<20) | (l.args[1].argI<<16);
            this->addIntoMachineCode(code);
          }
          else if (l.args[1].argS==""){
            // [reg+literal]
            // 1 - izvorisni, 2 - literal, 3 - odredisni
            // odredisni, izvorisni, literal
            int literal = l.args[2].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
              // literal can't fit in 12 bits
              outfile<<"Line "<<l.lineNum<<": Literal can't fit in 12 bits"<<endl;
            }
            int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[3].argI<<20) | (l.args[1].argI<<16) | (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // [reg + symbol]
            outfile<<"Line "<<l.lineNum<<": End value of symbol unknown during assembly!!"<<endl;
            exit(-1);
          }
        }

        else if (l.args[0].argS==""){
          // ld literal, gpr
          int literal = l.args[0].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)){
            // literal can't fit in 12b
            LiteralTableEntry lte = this->getLiteralTableEntry(literal);
            int disp = lte.getLocation()-(this->locationCounter+4);
            if (disp>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[1].argI<<20) | (Reg::PC<<16) | (disp & 0x00000fff);
            this->addIntoMachineCode(code);
            int code1 = ((OpCode::C_LD | 0b0010)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16);
            this->addIntoMachineCode(code1);
          }
          else {
            // literal can fit in 12b
            int code = ((OpCode::C_LD | 0b0010)<<24) |(l.args[1].argI<<20)| (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }

        }

        else {
          // ld symbol, gpr
          string symbol = l.args[0].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
            // symbol native to current section
            int disp = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            outfile<<this->symbolTable[symbolIndex].getValue()<<", "<<this->locationCounter<<endl;
            if (disp>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[1].argI<<20) | (Reg::PC<<16) | (disp & 0x00000fff);
            this->addIntoMachineCode(code);
            // add nop instruction add r0,r0 for example
            int code1 = ((OpCode::C_ARITH | 0b0000)<<24);
            this->addIntoMachineCode(code1);
          }
          else {
            // symbol not native to current section and/or .extern
            int location = this->addSymbolIntoLiteralTable(symbolIndex);
            this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
            int disp = location-(this->locationCounter+4);
            if (disp>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_LD | 0b0010)<<24) | (l.args[1].argI<<20) | (Reg::PC<<16) | (disp & 0x00000fff);
            this->addIntoMachineCode(code);
            int code1 = ((OpCode::C_LD | 0b0010)<<24) | (l.args[1].argI<<20) | (l.args[1].argI<<16);
            this->addIntoMachineCode(code1);
          }
        }
      }

      else if (l.name=="st"){
        if (l.args[1].argS=="$"){
          outfile<<"Line "<<l.lineNum<<": $literal or $symbol makes no sense!!"<<endl;
          exit(-1);
        }
        
        else if (l.args[1].argS=="%"){
          int code = ((OpCode::C_LD | 0b0001)<<24) | (l.args[2].argI<<20) | (l.args[0].argI<<16);
          this->addIntoMachineCode(code);
        }

        else if(l.args[1].argS=="[]"){
          if (l.args.size()==3){
            // [reg]
            int code = (OpCode::C_ST <<24) | (l.args[2].argI<<20) | (l.args[0].argI<<12);
            this->addIntoMachineCode(code);
          }
          else if (l.args[2].argS==""){
            // [reg+literal]
            int literal = l.args[3].argI;
            if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
              outfile<<"Line "<<l.lineNum<<": Literal for [reg+literal] can't be bigger than 12b!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_ST | 0b0000)<<24) | (l.args[2].argI<<20) | (l.args[0].argI<<12) | (literal & 0x00000fff);
            this->addIntoMachineCode(code);

          }
          else {
            // [reg+symbol]
            outfile<<"Line "<<l.lineNum<<": [reg+symbol] won't work!!"<<endl;
            exit(-1);
          }
        }

        else if (l.args[1].argS==""){
          // st gpr, literal
          int literal = l.args[1].argI;
          if (literal>pow(2,11)-1 || literal<1-pow(2,11)) {
            // literal can't fit into 12b
            LiteralTableEntry lte = this->getLiteralTableEntry(literal);
            int disp = lte.getLocation()-(this->locationCounter+4);
            if (disp>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_ST | 0b0010)<<24) | (Reg::PC<<20) | (l.args[0].argI<<12) | (disp & 0x00000fff);
            this->addIntoMachineCode(code);

          }
          else {
            // literal can fit into 12b
            int code = (OpCode::C_ST << 24) | (l.args[0].argI<<12)  | (literal & 0x00000fff);
            this->addIntoMachineCode(code);
          }

        }

        else {
          // st gpr, symbol
          string symbol = l.args[1].argS;
          int symbolIndex = this->checkIfSymbolInTable(symbol);
          if (this->symbolTable[symbolIndex].getSectionIndex()==this->getSectionIndex(this->currentSection)){
            // symbol native to current section
            int disp = this->symbolTable[symbolIndex].getValue()-(this->locationCounter+4);
            if (disp>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = (OpCode::C_ST<<24) | (Reg::PC<<20) | (l.args[0].argI<<12) | (disp & 0x00000fff);
            this->addIntoMachineCode(code);
          }
          else {
            // symbol not native to current section and/or .extern
            int location = this->addSymbolIntoLiteralTable(symbolIndex);
            this->addRelocIntoTable(this->currentSection, location, RelocType::R_32, symbolIndex);
            int disp = location - (this->locationCounter+4);
            if (disp>(pow(2,12)-1)){
              outfile<<"Line "<<l.lineNum<<": Maximum section size is 12b, can't access literal pool that's this far!!"<<endl;
              exit(-1);
            }
            int code = ((OpCode::C_ST | 0b0010)<<24) | (Reg::PC<<20) | (l.args[0].argI<<12) | (disp & 0x00000fff);
            this->addIntoMachineCode(code);
          }
        }
      }



      else {
        outfile<<"Line "<<l.lineNum<<": Unsupported command!!"<<endl;
        exit(-1);
      }
      
    }

    else {
      outfile<<"Line "<<l.lineNum<<": Unsupported line type!!"<<endl;
      exit(-1);
    }
  }

}

void Assembler::setOutputFile(string out){
  this->outFileName = out;
  this->outfile.open(out);

}

void Assembler::writeIntoBinaryFile(){
  string temp = this->outFileName;
  temp[temp.size()-1] = 'b';
  temp.push_back('i');temp.push_back('n');

  ofstream of;
  of.open(temp, ios::binary | ios::out);

  // write sections
  int secNum = this->sectionTable.size();
  of.write((char*)&secNum, sizeof(int));
  for (auto section:this->sectionTable){
    int x = section.getIndex();
    of.write((char*)&x, sizeof(int));
    string nzm = section.getName();
    char file[nzm.length()+1];
    strcpy(file, nzm.c_str());
    of.write(file, sizeof(file));
    uint y = section.getBase();
    of.write((char*)&y, sizeof(uint));
    y = section.getLength();
    of.write((char*)&y, sizeof(uint));
    if (this->literalTables[section.getName()].size()!=0){
     x = this->literalTables[section.getName()][0].getLocation();
    }
    else {
      x = -1;
    }
    of.write((char*)&x, sizeof(int));
    
  }

  // write symbols
  int symNum = this->symbolTable.size();
  of.write((char*)&symNum, sizeof(int));
  for (auto symbol:this->symbolTable){
    int x = symbol.getIndex();
    of.write((char*)&x, sizeof(int));
    uint y = symbol.getValue();
    of.write((char*)&y, sizeof(uint));
    x = (symbol.getType()==Type::NOTYP?0:1);
    of.write((char*)&x, sizeof(int));
    x = (symbol.getBind()==Bind::GLOB?0:1);
    of.write((char*)&x, sizeof(int));
    x = symbol.getSectionIndex();
    of.write((char*)&x, sizeof(int));
    string nzm = symbol.getName();
    char file[nzm.length()+1];
    strcpy(file, nzm.c_str());
    of.write(file, sizeof(file));
    
  }

  // write relocs
  int relocTablesNum = this->relocTables.size();
  of.write((char*)&relocTablesNum, sizeof(int));
  for (auto table:this->relocTables){
    int x = table.second.size();
    of.write((char*)&x, sizeof(int));
    x = this->getSectionIndex(table.first);
    of.write((char*)&x, sizeof(int));
    for (auto entry:table.second){
      x = entry.getOffset();
      of.write((char*)&x, sizeof(int));
      x = (entry.getType()==RelocType::R_32?0:1);
      of.write((char*)&x, sizeof(int));
      x = entry.getSymbolIndex();
      of.write((char*)&x, sizeof(int));
      x = entry.getAddend();
      of.write((char*)&x, sizeof(int));
    }
  }

  // write machine codes
  //int aaa = 0;
  int machineCodesNum = this->machineCodes.size();
  of.write((char*)&machineCodesNum, sizeof(int));
  for (auto mcs:this->machineCodes){
    int x = this->getSectionIndex(mcs.first);
    of.write((char*)&x, sizeof(int));
    for (char c:mcs.second){
       of.write((char*)&c, sizeof(char));
    }
  }

  of.close();

}

void Assembler::finishSecondPass(){
  this->sectionTable[this->getSectionIndex(this->currentSection)].setLength(this->locationCounter);
  
  for (auto& section:this->sectionTable){
    int lastLiteralTableLocation = 0;
    if (this->literalTables[section.getName()].size()>0){
      lastLiteralTableLocation = this->literalTables[section.getName()].back().getLocation()+4;
    }
    if (section.getLength()<lastLiteralTableLocation){
      section.setLength(lastLiteralTableLocation);
    }
    for(auto literal:this->literalTables[section.getName()]){
      this->addIntoSpecificMachineCode(literal.getValue(), section.getName());
    }
  }
}

void Assembler::addSymbolIntoTable(int value, Type type, Bind bind, int sectionIndex, string name){
  if (sectionIndex==-1){
    // trying to add symbol from .global
    for (auto& symbol:this->symbolTable){
      if (symbol.getName()==name && symbol.getSectionIndex()!=-2){
        // symbol already in table as local; set bind to GLOB
        symbol.setBind(Bind::GLOB);
        return;
      }
      else if (symbol.getName()==name && symbol.getSectionIndex()==-2){
        // symbol already added via .extern - error
        outfile<<"Symbol can't be added via .global if already added via .extern"<<endl;
        exit(-1);
      }

    }
    // symbol not already in table, add it
    SymbolTableEntry s;
    s.setValue(value);
    // i don't think type matters for now, can set it later
    s.setType(Type::NOTYP);
    s.setBind(Bind::GLOB);
    // setting sectionIndex as -1 which suggests that the symbol hasn't been defined already
    s.setSectionIndex(-1);
    s.setName(name);
    this->symbolTable.push_back(s);
    return;
  }
  else if (sectionIndex==-2){
    // trying to add from .extern
    for (auto& symbol:this->symbolTable){
      if (symbol.getName()==name){
        // symbol already in table - error
        outfile<<"Symbol can't be extern and local/global at the same time!!"<<endl;
        exit(-1);
      }
    }
    // symbol not in table, add it
    SymbolTableEntry s;
    s.setValue(0);
    s.setType(Type::NOTYP);
    s.setBind(Bind::GLOB);
    // setting sectionIndex as -2 which suggests that the symbol was added via .extern
    s.setSectionIndex(-2);
    s.setName(name);
    this->symbolTable.push_back(s);
  }
  else {
    // adding either from label: or .section 
    for (auto& symbol:this->symbolTable){
      if (symbol.getName()==name){
        // symbol already in table
        if (symbol.getSectionIndex()==-1){
          // added via .global
          // change value, type, sectionIndex
          symbol.setValue(value);
          symbol.setType(type);
          symbol.setSectionIndex(sectionIndex);
          return;
        }
        else {
          // added via regular symbol or .extern
          // error
          outfile<<"Multiple definitions of symbol or already declared as .extern"<<name<<endl;
          exit(-1);
        }
      }
    }
    // symbol not already in table
    SymbolTableEntry s;
    s.setValue(value);
    s.setType(type);
    s.setBind(bind);
    s.setSectionIndex(sectionIndex);
    s.setName(name);
    this->symbolTable.push_back(s);
    return;
  }

}

int Assembler::getSectionIndex(string name){
  int index = -2;
  for (auto section:this->sectionTable){
    if (section.getName()==name) {
      index = section.getIndex();
      break;
    }
  }
  if (index==-2){
    // section not in table
    // my error
    outfile<<"Section not in table or the command trying to write into section not in any section!!"<<endl;
    exit(-2);
  }
  else return index;
}

int Assembler::addSectionIntoTable(int locationCounter, string currentSection, string name, int base){
    // set the length of the currentSection to locationCounter
    // add new section with name name and base=0

    // set stuff in literal table

    for (auto& entry:this->literalTables[this->currentSection]){
      entry.setLocation(this->locationCounter);
      this->locationCounter+=4;
    }


    for (auto& section:this->sectionTable){
      if (section.getName()==name){
        outfile<<"Multiple definition of section!!"<<endl;
        exit(-1);
      }
      if (section.getName()==currentSection){
        section.setLength(locationCounter);
      }
    }
    

    SectionTableEntry s;
    s.setName(name);
    s.setBase(0);
    s.setLength(0);
    this->sectionTable.push_back(s);
    return s.getIndex();
}

void Assembler::addLiteralIntoTable(int value){
    for (auto literal:this->literalTables[this->currentSection]){
      if (literal.getValue()==value && literal.getName()==""){
        // literal already in table
        return;
      }
    }
    // literal not in table
    LiteralTableEntry l;
    l.setValue(value);
    l.setLocation(0);
    l.setName("");
    this->literalTables[this->currentSection].push_back(l);
}

int Assembler::addSymbolIntoLiteralTable(int symbolIndex){
  SymbolTableEntry symbol = this->symbolTable[symbolIndex];
  int address = -1;
  if (this->literalTables[this->currentSection].size()==0){
    address = this->sectionTable[this->getSectionIndex(this->currentSection)].getLength();
    LiteralTableEntry lte;
    lte.setValue(0);
    lte.setName(symbol.getName());
    lte.setLocation(address);
    this->literalTables[this->currentSection].push_back(lte);
    return address;
  }

  for (int i=0;i<this->literalTables[this->currentSection].size();i++){
    if (this->literalTables[this->currentSection][i].getName()==symbol.getName()){
      address = this->literalTables[this->currentSection][i].getLocation();
      return address;
    }
    if (i==this->literalTables[this->currentSection].size()-1){
      address = this->literalTables[this->currentSection][i].getLocation()+4; 
    }
  }

  LiteralTableEntry lte;
  lte.setValue(0);
  lte.setName(symbol.getName());
  lte.setLocation(address);
  this->literalTables[this->currentSection].push_back(lte);
  return address;

}


int Assembler::checkIfSymbolInTable(string symbol){
  int index = -3;
  for (auto s:this->symbolTable){
    if (symbol==s.getName()){
      index = s.getIndex();
      break;
    }
  }

  if (index==-3){
    outfile<<"Symbol "<<symbol<<" not in table!!"<<endl;
    exit(-1);
  }

  return index;
}

void Assembler::addRelocIntoTable(string section, int relocPlace, RelocType rType, int symbolIndex){
  RelocTableEntry rte;
  rte.setOffset(relocPlace);
  rte.setType(rType);
  if (rType==RelocType::R_32){
    if (this->symbolTable[symbolIndex].getBind()==Bind::GLOB){
      // symbol is GLOBAL, can be used in reloc table
      rte.setSymbolIndex(symbolIndex);
      rte.setAddend(0);
    }
    else {
      // symbol is LOCAL, cannot be used in reloc table
      // section must be used
      //int sectionIndex = this->symbolTable[symbolIndex].getSectionIndex();
      // WE USE INDEX FROM SYMBOL TABLE NOT SECTION TABLE!!
      int sectionIndex = -1;
      for (auto symbol:this->symbolTable){
        if (symbol.getName()==this->sectionTable[this->symbolTable[symbolIndex].getSectionIndex()].getName()) {
          sectionIndex = symbol.getIndex();
          break;
        }
      } 
      rte.setSymbolIndex(sectionIndex);
      rte.setAddend(this->symbolTable[symbolIndex].getValue());
    }
  }

  this->relocTables[section].push_back(rte);

}

void Assembler::addIntoMachineCode(int value, int numOfBytes){
  int mask = 0b11111111;
  for (int i=0;i<numOfBytes;i++){
    this->machineCodes[this->currentSection].push_back((value & mask)>>(i*8));
    this->locationCounter+=1;
    mask = mask<<8;
  }

}

 void Assembler::addIntoSpecificMachineCode (int value, string section, int numOfBytes){
  int mask = 0b11111111;
  for (int i=0;i<numOfBytes;i++){
    this->machineCodes[section].push_back((value & mask)>>(i*8));
    this->locationCounter+=1;
    mask = mask<<8;
  }
 }

LiteralTableEntry Assembler::getLiteralTableEntry(int value){
  for (auto literal: this->literalTables[this->currentSection]){
    if (literal.getValue()==value){
      return literal;
    }
  }

  return LiteralTableEntry();
}

LiteralTableEntry Assembler::getLiteralTableEntryLabel(string label){
  for(auto lte:this->literalTables[this->currentSection]){
    if (lte.getName()==label){
      return lte;
    }
  }

  return LiteralTableEntry();
}
