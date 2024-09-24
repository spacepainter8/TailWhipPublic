#include "../inc/linker.hpp"


void Linker::addInputFile(string inFile){
  inFile[inFile.size()-1]='b';
  inFile.push_back('i');inFile.push_back('n');
  this->inputFileNames.push_back(inFile);
  
  ifstream* in = new ifstream();
  
  in->open(inFile, ios::binary | ios::in);
  
  this->inputFiles.push_back(in);
  
}

void Linker::setOutputFile(string outFile){
  this->outputFile = outFile;
}

void Linker::addPlaceOption(string section, const char* address){
  //this->placeSections[section]=address;
  //yylval.literal = stoi(yytext+2, nullptr, 10);
  string temp = address;
  try {
    if (address[0]=='0' && (address[1]=='x' || address[1]=='X')){
    long lAdd = stol(temp.c_str()+2, nullptr, 16);
    if (lAdd>pow(2,32)-1 || lAdd<0) {
      cout<<"Place address too big or negative - "<<section<<endl;
      exit(-1);
    }
    uint add = lAdd;
    this->placeSections[section] = add; 
  }
  else if (address[0]=='0' && address[1]=='b'){
    long lAdd = stol(temp.c_str()+2, nullptr, 2);
    if (lAdd>pow(2,32)-1 || lAdd<0) {
      cout<<"Place address too big or negative - "<<section<<endl;
      exit(-1);
    }
    uint add = lAdd;
    this->placeSections[section] = add;
  }
  else if (address[0]=='0'){
    long lAdd = stol(temp.c_str()+2, nullptr, 8);
    if (lAdd>pow(2,32)-1 || lAdd<0) {
      cout<<"Place address too big or negative - "<<section<<endl;
      exit(-1);
    }
    uint add = lAdd;
    this->placeSections[section] = add;
  }
  else {
    long lAdd = stol(temp.c_str()+2, nullptr, 10);
    if (lAdd>pow(2,32)-1 || lAdd<0) {
      cout<<"Place address too big or negative - "<<section<<endl;
      exit(-1);
    }
    uint add = lAdd;
    this->placeSections[section] = add;
  }  
  } catch (std::exception){
    cout<<"Check -place option! Couldn't convert to number!!"<<endl;
    exit(-1);
  }
  
}

void Linker::startLinking(){

    if (inputFiles.size()==0){
      cout<<"NO INPUT FILES!!"<<endl;
      exit(-1);
    }
    if (outputFile=="") this->outputFile="output.hex";
    
    
    int i=0;
    for (auto file:this->inputFiles){
      
      this->readInputFile(i, file);
      i++;
    }

     
  // THIS IS FOR TESTING OUT LINKER OPTIONS
/*
    // cout<<"Hex set? "<<hexSet<<endl;
    // for (auto in:this->inputFileNames){
    //   cout<<in<<endl;
    // }
    // cout<<endl;
    // cout<<"OUTPUT FILE "<<this->outputFile<<endl;
    // cout<<"PLACE SECTIONS"<<endl;
    // for(auto entry:this->placeSections){
    //   cout<<entry.first<<" : "<<entry.second<<endl;
    // }
*/

  // THIS IS FOR TESTING THE INPUT OF THE FILES
/*    
  //   cout<<"Section tables"<<endl;  
  //   for (auto entry:sectionTables){
      
  //     cout<<endl<<entry.first<<endl;
  //     for (auto temp:entry.second){
  //       cout<<temp<<endl;
  //     }
  //   }

  // cout<<endl<<"symbol table"<<endl;
  // for (auto entry:symbolTables){
      
  //     cout<<endl<<entry.first<<endl;
  //     for (auto temp:entry.second){
  //       cout<<temp<<endl;
  //     }
  // }

  // cout<<endl<<"reloc tables"<<endl;
  // for (auto x:relocTables){
  //   cout<<x.first<<endl;
  //   for(auto y:x.second){
  //     cout<<y.first<<endl;
  //     for (auto z:y.second){
  //       cout<<z<<endl;
  //     }
  //   }
  // }

  // cout<<"machine codes"<<endl;
  // for (auto x:this->machineCodes){
  //   cout<<x.first<<endl;
  // for (auto machineTable:x.second){
  //       int location=0;
  //       cout<<machineTable.first<<endl;
  //       //outfile<<location<<": ";
  //       for(char machine:machineTable.second){
  //         char hex[9];
  //         snprintf(hex, 9, "%02X", machine);
  //         if (machine<0){
  //           if (location%8==0) cout<<endl<<location<<": ";
  //           cout<<hex[6]<<hex[7]<<" ";
  //           location++;
  //           if (location%4==0) {cout<<"\t";}
  //         }
  //         else {
  //           if (location%8==0) cout<<endl<<location<<": ";
  //           cout<<hex[0]<<hex[1]<<" ";
  //           location++;
  //           if (location%4==0) cout<<"\t";

            
  //         }
  //       }
  //       cout<<endl;
       
  //       }
  // }
 */ 

  //cout<<"WE HERE!!"<<endl;

  this->mapSections(); // should be good imo

  // THIS IS FOR TESTING OUT THE MAPPING OF THE SECTIONS
/*  
  for (auto section:sectionMap){
    cout<<get<0>(section);
    cout<<endl;
    for (auto entry:get<1>(section)){
      cout<<entry<<endl;
    }
  }
 cout<<endl<<endl<<endl<<endl;
 
*/

  // check if memory bigger than 2^32

  ulong sum = 0;
  for (auto section:this->sectionTable){
    sum+=section.getLength();
  }
  if (sum>pow(2,32)){
    cout<<"Sum of section lengths bigger than 2^32!!"<<endl;
    exit(-1);
  }





  // resolve symbols

  this->resolveSymbols();

// A BIT OF TESTING
/*
  cout<<"SECTION TABLE"<<endl;
  for (auto section:this->sectionTable){
    cout<<section<<endl;
  }

  cout<<endl<<endl<<endl<<"SYMBOL TABLE"<<endl;
  for (auto symbol:this->symbolTable){
    cout<<symbol<<endl;
  }
*/


  this->resolveRelocations();

// TESTING THE RELOCATIONS
/*
  for (auto mCodes:machineCodes){
    cout<<mCodes.first<<endl;
    for (auto machines:mCodes.second){
      cout<<machines.first<<endl;
      int location = 0;
      for (auto chars:machines.second){
        char hex[9];
          snprintf(hex, 9, "%02X", chars);
          if (chars<0){
            if (location%8==0) cout<<endl<<location<<": ";
            cout<<hex[6]<<hex[7]<<" ";
            location++;
            if (location%4==0) {cout<<"\t";}
          }
          else {
            if (location%8==0) cout<<endl<<location<<": ";
            cout<<hex[0]<<hex[1]<<" ";
            location++;
            if (location%4==0) cout<<"\t";

            
          }
      }
    }
  }
*/

if (this->hexSet){
  
  ofstream outfile;
  outfile.open(this->outputFile);
  string outputBinary = this->outputFile;
  outputBinary[outputBinary.size()-3]='b';
  outputBinary[outputBinary.size()-2]='i';
  outputBinary[outputBinary.size()-1]='n';
  //ofstream binary;
  //binary.open(outputBinary, ios::binary | ios::out);
  
  int i; int fd; char* map;
  // ovo mozda mora da se podesi
  outputBinary = "/home/ss/Desktop/Projekat/" + outputBinary;
  char temp[outputBinary.length()+1];
  strcpy(temp, outputBinary.c_str());
  fd = open(temp, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
  lseek(fd, sizeof(char)*pow(2, 32), SEEK_SET);
  write(fd, "", 1);
  map = (char*)mmap(0, sizeof(char)*pow(2, 32), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);



  
  int numOfChars = get<1>(this->sectionMap.back()).back().getBase()+get<1>(this->sectionMap.back()).back().getLength();
  int bigcnt = 0;
  while(numOfChars>=10){
    numOfChars = numOfChars/10;
    bigcnt++;
  }
  bigcnt++;
  uint outputNum = 0;
  for (auto section:this->sectionTable){
      
      uint location = section.getBase();
      
    for (auto mCode:this->machineCodes){
      for (auto innerSection:mCode.second){
        if (innerSection.first==section.getName()){
            for (auto code:innerSection.second){
              //binary.write((char*)&code, sizeof(char));
             map[location] = code;
              char hex[9];
          snprintf(hex, 9, "%02X", code);
          if (code<0){
            if (outputNum%8==0) {
              // int smallCnt = 0;
              // uint locationTemp = location;
              // while(locationTemp>=10){
              //   locationTemp = locationTemp/10;
              //   smallCnt++;
              // }
              // smallCnt++;
              outfile<<endl;
              //outfile<<hex<<location<<": ";
              string s = "00000000";
              sprintf(&(s[0]), "%08X", location);
              outfile<<s<<": "; 

               
              // int cnt = bigcnt - smallCnt;
              // while (cnt>0) {outfile<<" ";cnt--;}
            }
            outfile<<hex[6]<<hex[7]<<" ";
            
            location++;
            
            outputNum++;
            if (outputNum%4==0) {outfile<<"\t";}
          }
          else {
            if (outputNum%8==0) {
              // int smallCnt = 0;
              // uint locationTemp = location;
              // while(locationTemp>=10){
              //   locationTemp = locationTemp/10;
              //   smallCnt++;
              // }
              // smallCnt++;
              outfile<<endl;
              //outfile<<hex<<location<<": ";
              string s = "00000000";
              sprintf(&(s[0]), "%08X", location);
              outfile<<s<<": ";
              // int cnt = bigcnt - smallCnt;
              // while (cnt>0) {outfile<<" ";cnt--;}
            }
            outfile<<hex[0]<<hex[1]<<" ";
            location++;
            outputNum++;
            if (outputNum%4==0) outfile<<"\t";

            }
            
        }
        break;
      }
    }
  }
}
//binary.close();
munmap(map, sizeof(char)*pow(2, 32));
close(fd);
outfile.close();
}


  for (auto file:this->inputFiles){
    file->close();
  }

  // unmap and close outputfiles
}
void Linker::readInputFile(int fileNum, ifstream* in){
  // read section table
  // read number of sections
  int numberOfSections = 0;
  in->read((char*)&numberOfSections, sizeof(int));
   // cout<<numberOfSections<<endl;
  for (int i=0;i<numberOfSections;i++){
    SectionTableEntry s;
    int temp;
    // read section index
    in->read((char*)&temp, sizeof(int));
    s.setIndex(temp);
    // read section name
    string x = "";
    char c = 'a';
    while(true){
      in->read(&c, 1);
      if (c=='\0'){
        break;
      }
      x.push_back(c);
    };
    s.setName(x);
    // read base
    uint tempU;
    in->read((char*)&tempU, sizeof(uint));
    s.setBase(tempU);
    // read length
    in->read((char*)&tempU, sizeof(uint));
    s.setLength(tempU);
    // read starting address of literal pool
    in->read((char*)&temp, sizeof(int));
    // setting this into base, whatever
    // if -1 there is no literal table, if it positive it is the address 
    // of the literal table
    s.setBase(temp);
    this->sectionTables[fileNum].push_back(s);
  };


   

  // read symbol table
  // read number of symbols
  int numberOfSymbols = 0;
  in->read((char*)&numberOfSymbols, sizeof(int));
  for (int i=0;i<numberOfSymbols;i++){
    SymbolTableEntry s;
    int temp;
    // read symbol index
    in->read((char*)&temp, sizeof(int));
    s.setIndex(temp);
    // read symbol 
    uint tempU;
    in->read((char*)&tempU, sizeof(uint));
    s.setValue(tempU);
    // read symbol type
    in->read((char*)&temp, sizeof(int));
    if (temp==0) s.setType(Type::NOTYP);
    else s.setType(Type::SCTN);
    // read symbol bind
    in->read((char*)&temp, sizeof(int));
    if (temp==0) s.setBind(Bind::GLOB);
    else s.setBind(Bind::LOC);
    // read symbol section index
    in->read((char*)&temp, sizeof(int));
    s.setSectionIndex(temp);
    // read symbol name
    string x = "";
    char c = 'a';
    while(true){
      in->read(&c, 1);
      if (c=='\0'){
        break;
      }
      x.push_back(c);
    };
    s.setName(x);
    this->symbolTables[fileNum].push_back(s);
  }

   

 

  // read reloc tables
  // read number of reloc tables
  int numberOfRelocTables = 0;
  in->read((char*)&numberOfRelocTables, sizeof(int));
  for (int i=0;i<numberOfRelocTables;i++){
    // read number of entries in this table
    int numberOfEntries = 0;
    in->read((char*)&numberOfEntries, sizeof(int));
    // read for which section this table is
    int sectionNumber = 0;
    in->read((char*)&sectionNumber, sizeof(int));
    // get sectio name from index
    string sectionName = "";
    for(auto section:this->sectionTables[fileNum]){
      if (section.getIndex()==sectionNumber){
        sectionName = section.getName();
        break;
      }
    }
    for (int i=0;i<numberOfEntries;i++){
      RelocTableEntry r;
      int temp=0;
      // read offset
      in->read((char*)&temp, sizeof(int));
      r.setOffset(temp);
      // read type
      in->read((char*)&temp, sizeof(int));
      if (temp==0) r.setType(RelocType::R_32);
      // read symbol index
      in->read((char*)&temp, sizeof(int));
      r.setSymbolIndex(temp);
      // read addend 
      in->read((char*)&temp, sizeof(int));
      r.setAddend(temp);
      this->relocTables[fileNum][sectionName].push_back(r);
    }
  }

  

  


  // read machine codes

  int numberOfMachineCodes = 0;
  in->read((char*)&numberOfMachineCodes, sizeof(int));
  //cout<<numberOfMachineCodes<<endl;
  
  
  for (int i=0;i<numberOfMachineCodes;i++){
    // get sectionIndex
    int sectionIndex = -3;
    in->read((char*)&sectionIndex, sizeof(int));
    // get sectionname from index
    string sectionName = "";
    uint sectionSize = 0;
    for(auto section:this->sectionTables[fileNum]){
      if (section.getIndex()==sectionIndex){
        sectionName = section.getName();
        sectionSize = section.getLength();
        break;
      }
    }
    
    for(uint i=0;i<sectionSize;i++){
      char c;
      in->read((char*)&c, sizeof(char));
      this->machineCodes[fileNum][sectionName].push_back(c);
    }
    
  }

  


}

void Linker::mapSections() {
  // map section from -place first
  for (auto place:this->placeSections){
    tuple<string, vector<SectionTableEntry>> torka;
    torka = make_tuple(place.first, vector<SectionTableEntry>());
    uint address = place.second;
    for (auto& sectionTable:this->sectionTables){
      for (auto& section:sectionTable.second){
          
        if (section.getName()==place.first){
          SectionTableEntry ste;
          ste.setIndex(sectionTable.first);
          ste.setBase(address);
          address+=section.getLength();
          ste.setLength(section.getLength());
          ste.setName(section.getName());
          get<1>(torka).push_back(ste);
          // this means that the section has already been mapped
          section.setBase(-5);
        }
      }
    }
    
  this->sectionMap.push_back(torka);
  }

// now sort sectionMap by base address of first element in vector from tuple

  // delete if sections from place don't exist in files
  vector<tuple<string, vector<SectionTableEntry>>> temp;
  for (int i=0;i<sectionMap.size();i++){
    if (get<1>(this->sectionMap[i]).size()!=0){
      temp.push_back(this->sectionMap[i]);
    }
  }
  this->sectionMap = temp;

  for (int i=0;i<sectionMap.size();i++){
  
    for (int j=i+1;j<sectionMap.size();j++){
      
      if ((get<1>(this->sectionMap[i]))[0].getBase()>(get<1>(this->sectionMap[j]))[0].getBase()){
        tuple<string, vector<SectionTableEntry>> temp = this->sectionMap[i];
        this->sectionMap[i] = this->sectionMap[j];
        this->sectionMap[j] = temp;
      }
      
   
    }
  }

// now check if sections from place overlap eachother
// last element from previous section (base+length)>first element from next section.base?
// if so, overlapping

  for (int i=0;i<sectionMap.size();i++){
   uint previousSectionEnd =  get<1>(this->sectionMap[i]).back().getBase()+get<1>(this->sectionMap[i]).back().getLength();
    if (i<(sectionMap.size()-1)){
      uint nextSectionStart = get<1>(this->sectionMap[i+1])[0].getBase();
      if (previousSectionEnd>nextSectionStart){
        cout<<"Sections from place overlapping!!"<<endl;
        cout<<(get<1>(this->sectionMap[i])).back().getName()<<" and "<<get<1>(this->sectionMap[i+1])[0].getName()<<endl;
        exit(-1);
      }
    }
  }

// now add the rest of the sections
// don't forget to set base to -5 after mapping a section

  for (int i=0;i<sectionTables.size();i++){
    for(int j=0;j<sectionTables[i].size();j++){
      if (sectionTables[i][j].getBase()!=-5){
        // add into sectionMap
        // check for same section in other maps
        vector<SectionTableEntry> steVector;
        SectionTableEntry ste;
        uint newAddress = -1;
        if (sectionMap.size()==0){
          ste.setBase(0);
        }
        else {
          newAddress = (get<1>(sectionMap.back())).back().getBase()+(get<1>(sectionMap.back())).back().getLength();
          ste.setBase(newAddress);
        }
        ste.setIndex(i);
        ste.setLength(sectionTables[i][j].getLength());
        ste.setName(sectionTables[i][j].getName());
        steVector.push_back(ste);
        // now check in other files for the same section
        for (int k=i+1;k<sectionTables.size();k++){
          for (int s = 0;s<sectionTables[k].size();s++){
            if (sectionTables[k][s].getName()==ste.getName()){
              ste.setBase(ste.getBase()+ste.getLength());
              ste.setIndex(k);
              ste.setLength(sectionTables[k][s].getLength());
              steVector.push_back(ste);
              sectionTables[k][s].setBase(-5);
            }
          }
        }
      
      tuple<string, vector<SectionTableEntry>> torka;
      torka = make_tuple(sectionTables[i][j].getName(), steVector);
      this->sectionMap.push_back(torka);
      sectionTables[i][j].setBase(-5);
      }
    }
  }

  
}

void Linker::resolveSymbols(){
  //  first make section table
  // make symbol table (only take symbols whose sectionIndex!=-2)
  // if symbol already in table == error!
  // check if all sectionIndex==2 symbols defined in new symbol table, if not == error!

  // create section table

  for (auto torka:this->sectionMap){
    SectionTableEntry ste;
    ste.setName(get<0>(torka));
    ste.setBase(get<1>(torka)[0].getBase());
    uint lengthSum = 0;
    for (auto entry:get<1>(torka)){
      lengthSum+=entry.getLength();
    }
    ste.setLength(lengthSum);
    this->sectionTable.push_back(ste);
    // need to add it to symbol table too
    SymbolTableEntry symte;
    symte.setValue(ste.getBase());
    symte.setType(Type::SCTN);
    symte.setBind(Bind::GLOB);
    symte.setSectionIndex(ste.getIndex());
    symte.setName(ste.getName());
    this->symbolTable.push_back(symte);

  }

  // create symbol table

  for (auto table:this->symbolTables){
    for (auto symbol:table.second){
      if (symbol.getType()==Type::NOTYP && symbol.getSectionIndex()!=-2){
        // check if symbol already defined
        if (symbol.getBind()==Bind::GLOB){
          this->checkIfSymbolDefined(symbol.getName());
        }
        // first get section name by index
        vector<SectionTableEntry> sectionTableForFile = this->sectionTables[table.first];
        string sectionName;
        for (auto section:sectionTableForFile){
          if (section.getIndex()==symbol.getSectionIndex()){
            sectionName = section.getName();
            break;
          }
        }
        // now get base of section specific for file from sectionMap
        uint base = -1;
        for (auto torka:this->sectionMap){
          if (get<0>(torka)==sectionName){
            for (auto section:get<1>(torka)){
              if (section.getIndex()==table.first){
                  base = section.getBase();
                  break;
              }
            }
            break;
          }
        }

        SymbolTableEntry ste;
        ste.setValue(base+symbol.getValue());
        ste.setType(Type::NOTYP);
        ste.setBind(symbol.getBind());
        // now get new sectionIndex from sectionTable
        for (auto section:this->sectionTable){
          if (section.getName()==sectionName){
            ste.setSectionIndex(section.getIndex());
            break;
          }
        }
        ste.setName(symbol.getName());
        ste.setFileNum(table.first);
        this->symbolTable.push_back(ste);
      }
    }
  }

  // check if all -2 are defined

  for (auto table:this->symbolTables){
    for (auto symbol:table.second){
      if (symbol.getSectionIndex()==-2){
        int index = -1;
        for (auto entry:this->symbolTable){
          if (symbol.getName()==entry.getName() && entry.getBind()==Bind::GLOB){
              index = 1;
              break;
          }
        }
        if (index==-1){
          cout<<"Symbol "<<symbol.getName()<<" not defined as global (or at all) in any other file!!"<<endl;
          exit(-1);
        }
      }
    }
  }



}

void Linker::checkIfSymbolDefined(string symbol){
  for (auto entry:this->symbolTable){
    if (entry.getName()==symbol & entry.getBind()==Bind::GLOB){
      cout<<"Symbol "<<symbol<<" already defined in another file!!"<<endl;
      exit(-1);
    }
  }
}

void Linker::resolveRelocations(){
  for (auto relocTable:this->relocTables){
    int fileNum = relocTable.first;
    for (auto table:relocTable.second){
      string sectionName = table.first;
      uint baseAddress = -1;
      // find base address for given section in given file
      for (auto sMap:this->sectionMap){
        if (get<0>(sMap)==sectionName){
          for (auto section:get<1>(sMap)){
            if (section.getIndex()==fileNum){
              baseAddress = section.getBase();
              break;
            }
          }
          break;
        }
      }
      for (auto reloc:table.second){
        // offset, type, symbolIndex, addend
        int relocPosition = reloc.getOffset();
        string symbolName = this->symbolTables[fileNum][reloc.getSymbolIndex()].getName();
        
        
        uint symbolValue;
        for (auto symbol:this->symbolTable){
          if (symbol.getName()==symbolName && symbol.getType()==Type::SCTN){
            for (auto map:this->sectionMap){
              if (get<0>(map)==symbol.getName()){
                for (auto entry:get<1>(map)){
                  if (entry.getIndex()==fileNum){
                    symbolValue = entry.getBase();
                    break;
                  }
                }
                break;
              }
            }
          
            break;
          }
          else if (symbol.getName()==symbolName && (symbol.getFileNum()==fileNum || (symbol.getFileNum()!=fileNum && symbol.getBind()==Bind::GLOB))){
            symbolValue = symbol.getValue();
            
            break;
          }
        }
        if (reloc.getType()==RelocType::R_32)
        symbolValue+=reloc.getAddend();
    //     int mask = 0b11111111;
    // for (int i=0;i<numOfBytes;i++){
    //   this->machineCodes[this->currentSection].push_back((value & mask)>>(i*8));
    //   this->locationCounter+=1;
    //   mask = mask<<8;
    // }
        int mask = 0b11111111;
        for (int i=0;i<4;i++){
          this->machineCodes[fileNum][sectionName][relocPosition] = (symbolValue & mask)>>(i*8);
          mask = mask<<8;
          relocPosition++;

        }
      }
      
    }
  }
}