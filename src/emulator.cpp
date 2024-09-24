#include "../inc/emulator.hpp"

void Emulator::setInputFile(char* in){
  this->inputFile = in;
  this->inputFile = "/home/ss/Desktop/Projekat/" + this->inputFile;
}

void Emulator::startEmulating(){
  this->initializeEmulator();

  // read 4 chars from map in pc, pc+1...
  // 1st char -> DispM DispL
  // 2nd char -> RegC DispH
  // 3rd char -> RegA RegB
  // 4th char -> OC MOD

  char one = 8;
  char two = 8;
  char three = 8;
  char four = 8;

  while(one!=0 || two!=0 || three!=0 || four!=0){

    one = this->mem[this->reg[Reg::PC]++];
    two = this->mem[this->reg[Reg::PC]++];
    three = this->mem[this->reg[Reg::PC]++];
    four = this->mem[this->reg[Reg::PC]++];
   


    // get disp
    uint disp = one & (0x000000ff);
    uint dispH = ((two& (0x000000ff)) & 0b00001111)<<8;
   
   

    disp = disp + dispH;
    uint twelvethBit = 0b100000000000;
    twelvethBit = twelvethBit & disp;
    if (twelvethBit==0b100000000000){
      // need to extend disp with 1
      disp = disp | 0xfffff000;

    }



    // get RegC
    int regC = ((two& (0x000000ff)) & 0b11110000)>>4;
    // get RegB
    int regB = ((three& (0x000000ff)) & 0b00001111);
    // get regA
    int regA = ((three& (0x000000ff)) & 0b11110000)>>4;
    // get MOD
    int mod = (four& (0x000000ff)) & 0b00001111;
    // get OC
    // leave oc with top 4 bits and zeros at the end cause im dumb
    int oc = (four& (0x000000ff)) & 0b11110000;

    // test out if i'm reading the file correctly
    //printf("%x\n %x\n %x\n %x\n %x\n %x\n", disp, regC, regB, regA, mod, oc);

    // first figure out the OC, then the MOD, then act accordingly
    // actually can check for register validity right away


    if (regA<0 || regA>15 || regB<0 || regB>15 || regC<0 || regC>15){
      this->ctrlReg[Reg::CAUSE] = 1;
      this->doInterruptStuff();
      continue;
    }

    

    switch(oc){
      
      case OpCode::C_HALT:{
        if (mod==0 && regA==0 && regB==0 && regC==0 && disp==0 ){
          // print registers
          cout<<"-----------------------------------------------------------------"<<endl;
          cout<<"Emulated processor executed halt instruction"<<endl;
          cout<<"Emulated processor state:"<<endl;
          for (int i=0;i<16;i++){
            if (i<10) cout<<" ";
            if (i<10 & i%4!=0) cout<<"    ";
            if (i>=10 & i!=12) cout<<"    ";
            //if (i%4!=0) cout<<"\t";
            cout<<"r"<<i<<"=0x";
            string s = "00000000";
            sprintf(&(s[0]), "%08x", this->reg[i]);
            cout<<s;
            if (i%4==3) cout<<endl;
          }
          
          // this is for testing purposes only
        /*
          cout<<"status: ";printf("%x\n", this->ctrlReg[Reg::STATUS]);
          cout<<"handler: ";printf("%x\n", this->ctrlReg[Reg::HANDLER]);
          cout<<"cause: ";printf("%x\n", this->ctrlReg[Reg::CAUSE]);
          cout<<"sp: ";printf("%x\n", this->reg[Reg::SP]);
          cout<<"pc: ";printf("%x\n", this->reg[Reg::PC]);
        */ 

        continue;
        }
        else {
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }
        break;
      }

      case OpCode::C_INT:{
        if (mod==0 && regA==0 && regB==0 && regC==0 && disp==0){
          this->ctrlReg[Reg::CAUSE] = 4;
          this->doInterruptStuff();
        }
        else {
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }
        break;
      }

      case OpCode::C_CALL:{
        if (regC!=0){
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }

        switch(mod){
          case 0b0000:{
            this->push(this->reg[Reg::PC]);
            this->reg[Reg::PC] = this->reg[regA]+this->reg[regB]+disp;
            break;
          }
          case 0b0001: {
            this->push(this->reg[Reg::PC]);
            uint index = this->reg[regA] + this->reg[regB] + disp;
            this->reg[Reg::PC] = this->readMem(index);
            break;
          }
          default:{
            this->ctrlReg[Reg::CAUSE] = 1;
            this->doInterruptStuff();
            continue;
          }
        }
        break;
      }

      case OpCode::C_JMP:{
        switch(mod){
          case 0b0000:{
            this->reg[Reg::PC] = this->reg[regA] + disp;
            break;
          }
          case 0b0001: {
            if (this->reg[regB]==this->reg[regC]) this->reg[Reg::PC] = this->reg[regA]+disp;
            break;
          }
          case 0b0010: {
            if (this->reg[regB]!=this->reg[regC]) this->reg[Reg::PC] = this->reg[regA]+disp;
            break;
          }
          case 0b0011:{
            int b = this->reg[regB];
            int c = this->reg[regC];
            if (b>c) this->reg[Reg::PC] = this->reg[regA]+disp;
            break;
          }
          case 0b1000:{
            this->reg[Reg::PC] = this->readMem(this->reg[regA]+disp);
            break;
          }
          case 0b1001:{
            if (this->reg[regB]==this->reg[regC]) this->reg[Reg::PC] = this->readMem(this->reg[regA]+disp);
            break;
          }
          case 0b1010:{
            if (this->reg[regB]!=this->reg[regC]) this->reg[Reg::PC] = this->readMem(this->reg[regA]+disp);
            break;
          }
          case 0b1011:{
            int b = this->reg[regB];
            int c = this->reg[regC];
            if (b>c) this->reg[Reg::PC] = this->readMem(this->reg[regA]+disp);
            break;
          }
          default:{
            this->ctrlReg[Reg::CAUSE] = 1;
            this->doInterruptStuff();
            continue;
          }
          
        }
        break;
      }

      case OpCode::C_XCHG:{
        if (mod!=0 || regA!=0 || disp!=0) {
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }

        uint temp = this->reg[regB];
        this->reg[regB] = this->reg[regC];
        this->reg[regC] = temp;
        
        // fix r0 if it was dirtied 
        this->reg[Reg::R0] = 0;

        break;
      }

      case OpCode::C_ARITH:{
        if (disp!=0){
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }

        switch(mod){
          case 0b0000:{
            this->reg[regA] = this->reg[regB]+this->reg[regC];
            break;
          }

          case 0b0001:{
            this->reg[regA] = this->reg[regB]-this->reg[regC];
            break;
          }

          case 0b0010:{
            this->reg[regA] = this->reg[regB] * this->reg[regC];
            break;
          }

          case 0b0011:{
            this->reg[regA] = this->reg[regB] / this->reg[regC];
            break;
          }

          default:{
            this->ctrlReg[Reg::CAUSE] = 1;
            this->doInterruptStuff();
            continue;
          }
        }
        
          // fix r0 if it was dirtied 
        this->reg[Reg::R0] = 0;


        break;
      }

      case OpCode::C_LOG:{
        if (disp!=0){
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }

        switch(mod){
          case 0b0000:{
            this->reg[regA] = ~(this->reg[regB]);
            break;
          }

          case 0b0001:{
            this->reg[regA] = this->reg[regB] & this->reg[regC];
            break;
          }

          case 0b0010:{
            this->reg[regA] = this->reg[regB] | this->reg[regC];
            break;
          }

          case 0b0011:{
            this->reg[regA] = this->reg[regB] ^ this->reg[regC];
            break;
          }
          
          default:{
            this->ctrlReg[Reg::CAUSE] = 1;
            this->doInterruptStuff();
            continue;
          }
        }
          // fix r0 if it was dirtied 
        this->reg[Reg::R0] = 0;
        break;
      }

      case OpCode::C_SHIFT:{
        if (disp!=0){
          this->ctrlReg[Reg::CAUSE] = 1;
          this->doInterruptStuff();
          continue;
        }

        switch(mod){
          case 0b0000:{
            this->reg[regA] = this->reg[regB] << this->reg[regC];
            break;
          }

          case 0b0001:{
            this->reg[regA] = this->reg[regB] >> this->reg[regC];
            break;
          }

          default:{
            this->ctrlReg[Reg::CAUSE] = 1;
            this->doInterruptStuff();
            continue;
          }
        }
          // fix r0 if it was dirtied 
        this->reg[Reg::R0] = 0;
        break;
      }

      case OpCode::C_ST:{
        switch(mod){
          case 0b0000:{
            this->writeMem(this->reg[regA]+this->reg[regB]+disp, this->reg[regC]);
            break;
          }

          case 0b0010:{
            this->writeMem(this->readMem(this->reg[regA]+this->reg[regB]+disp), this->reg[regC]);
            break;
          }

          case 0b0001:{
            this->reg[regA] = this->reg[regA]+disp;
              // fix r0 if it was dirtied 
             this->reg[Reg::R0] = 0;
            this->writeMem(this->reg[regA], this->reg[regC]);
            break;
          }

          default:{
            this->ctrlReg[Reg::CAUSE] = 1;
            this->doInterruptStuff();
            continue;
          }
        }
          // fix r0 if it was dirtied 
        this->reg[Reg::R0] = 0;
        break;
      }

      case OpCode::C_LD:{

        switch(mod){
          case 0b0000:{
            if (regA==0) continue;
            if (regB!=0 && regB!=1 && regB!=2){
              this->ctrlReg[Reg::CAUSE] = 1;
              this->doInterruptStuff();
              continue;
            }
            this->reg[regA] = this->ctrlReg[regB];
            break;
          }

          case 0b0001:{
            
            if (regA==0) continue;
            this->reg[regA] = this->reg[regB]+disp;
            break;
          }

          case 0b0010:{
            if (regA==0) continue;
            
            this->reg[regA] = this->readMem(this->reg[regB]+this->reg[regC]+disp);
           
            break;
          }

          case 0b0011:{
            if (regA==0) continue;
            this->reg[regA] = this->readMem(this->reg[regB]);
            this->reg[regB] = this->reg[regB]+disp;
            break;
          }

          case 0b0100:{
           
            if (regA!=0 && regA!=1 && regA!=2){
              this->ctrlReg[Reg::CAUSE] = 1;
              this->doInterruptStuff();
              continue;
            }

            this->ctrlReg[regA] = this->reg[regB];
            if (regA==Reg::HANDLER) this->handlerSet = true;
            break;
          }

          case 0b0101:{
            if ((regA!=0 && regA!=1 && regA!=2)||(regB!=0 && regB!=1 && regB!=2)){
              this->ctrlReg[Reg::CAUSE] = 1;
              this->doInterruptStuff();
              continue;
            }

            this->ctrlReg[regA] = this->ctrlReg[regB] | disp;
            if (regA==Reg::HANDLER) this->handlerSet = true;
            break;
          }

          case 0b0110:{
            if (regA!=0 && regA!=1 && regA!=2){
              this->ctrlReg[Reg::CAUSE] = 1;
              this->doInterruptStuff();
              continue;
            }

            this->ctrlReg[regA] = this->readMem(this->reg[regB]+this->reg[regC]+disp);
            if (regA==Reg::HANDLER) this->handlerSet = true;
            break;
          }

          case 0b0111:{
            if (regA!=0 && regA!=1 && regA!=2){
              this->ctrlReg[Reg::CAUSE] = 1;
              this->doInterruptStuff();
              continue;
            }

            this->ctrlReg[regA] = this->readMem(this->reg[regB]);
            if (regA==Reg::HANDLER) this->handlerSet = true;
            this->reg[regB] = this->reg[regB]+disp;
              // fix r0 if it was dirtied 
            this->reg[Reg::R0] = 0;
            break;
          }

        }
        
        break;
      }

      default:{
         this->ctrlReg[Reg::CAUSE] = 1;
         this->doInterruptStuff();
         continue;
      }


    } 








    
  }
  


}

void Emulator::initializeEmulator(){
   

  this->reg[Reg::PC] = 0x40000000;
  this->reg[Reg::SP] = 0;
  
  this->reg[Reg::R0] = 0;
  this->ctrlReg[Reg::STATUS]=0;
  this->ctrlReg[Reg::CAUSE] = 0;
  // need to actually open the file
 
  this->inputFile[inputFile.length()-3] = 'b';
  this->inputFile[inputFile.length()-2] = 'i';
  this->inputFile[inputFile.length()-1] = 'n';
  
  
  // char x[this->inputFile.length()+1];
  // strcpy(x, this->inputFile.c_str());

  const char* x = this->inputFile.c_str();
 
 

  this->fd = open(x, O_RDWR);


  this->mem = (char*)mmap(0, sizeof(char)*pow(2, 32), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  struct stat st;
  stat(x, &st);
  if (st.st_size>sizeof(char)*pow(2, 32)+1){
    //cout<<st.st_size<<endl;
    cout<<"File can't be loaded into memory!! (too big)"<<endl;
    exit(-1);
  }
  
  
}

void Emulator::doInterruptStuff(){
  // don't need to check status; don't have timer nor terminal
  // check if cause is valid
  // push status; push pc
  // push is sp<=sp-4; mem32[sp]<=gpr
  // pc = handler
  // status -> set lowest bit to 1

  if (this->handlerSet==false){
    cout<<"Handler not set!!"<<endl;
    exit(-1);
  }

  uint temp = this->ctrlReg[Reg::CAUSE];
  if (temp!=1 && temp!=2 && temp!=3 && temp!=4) return;
  this->push(this->ctrlReg[Reg::STATUS]);
  this->push(this->reg[Reg::PC]);
  this->reg[Reg::PC] = this->ctrlReg[Reg::HANDLER];
  this->ctrlReg[Reg::STATUS] = this->ctrlReg[Reg::STATUS] & (~0x1);


}

void Emulator::irregularOp(){
  this->ctrlReg[Reg::CAUSE] = 1;
}


void Emulator::push(uint value){
  this->reg[Reg::SP]-=4;
  uint temp = this->reg[Reg::SP];
  int mask = 0b11111111;
  for (int i=0;i<4;i++){
    // this->machineCodes[this->currentSection].push_back((value & mask)>>(i*8));
    // this->locationCounter+=1;
    this->mem[temp] = ((value&mask)>>(i*8)) & 0x000000ff;
    temp++;
    mask = mask<<8;
  }
}

uint Emulator::readMem(uint location){
  uint value = 0;
  int mask = 0b11111111;
  for (int i=0;i<4;i++){

    uint memVal = this->mem[location] & 0x000000ff;
    
    
    location++;
    memVal = memVal <<(i*8);
    value+=memVal;
  }

  return value;
}

void Emulator::writeMem(uint location, uint value){
  int mask = 0b11111111;

  for (int i=0;i<4;i++){  
    this->mem[location] = ((value&mask)>>(i*8)) & 0x000000ff;
    location++;
    mask=mask<<8;
  }
}
