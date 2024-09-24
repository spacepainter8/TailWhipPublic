#ifndef LINKER_HPP
#define LINKER_HPP

#include "./sectionTable.hpp"
#include "./symbolTableEntry.hpp"
#include "./relocTableEntry.hpp"


#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <unordered_map>
#include <map>
#include <iostream>
#include <tuple>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


using namespace std;

class Linker {
private:
  bool hexSet;
  vector<string> inputFileNames;
  // to do:: needs deleting
  vector<ifstream*> inputFiles;
  string outputFile;
  // key is section name, value is address from -place
  unordered_map<string, uint> placeSections;
  // this is the final mapping basically
  // key is section name, value is (index is file number, base is base, length is length)
  //unordered_map<string, vector<SectionTableEntry>> sectionMap;
  vector<tuple<string, vector<SectionTableEntry>>> sectionMap; 
  // <input file number, sectionTable>
  map<int, vector<SectionTableEntry>> sectionTables;
  // <input file number, symbolTable>
  map<int, vector<SymbolTableEntry>> symbolTables;
  // <input file number, <section name, relocTable>>
  map<int, unordered_map<string, vector<RelocTableEntry>>> relocTables;
  // <input file number, <section name, machineCode>>
  map<int, unordered_map<string, vector<char>>> machineCodes;

  vector<SectionTableEntry> sectionTable;
  vector<SymbolTableEntry> symbolTable;

public:
  void addInputFile(string inFile);
  void setOutputFile(string outFile);
  void addPlaceOption(string section, const char* address);
  void setHexSet (bool hs){
    this->hexSet = hs;
  }
  bool getHexSet () {return this->hexSet;}

  void startLinking();
  void readInputFile(int fileNum, ifstream* in);
  void mapSections();
  void resolveSymbols();
  void checkIfSymbolDefined(string symbol);
  void resolveRelocations();

};







#endif


// some stuff i may need

// citanje iz mmapa
/*
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
using namespace std;


int main(){
  
  int i;
  int fd;
  int result;
  char* map;

  fd = open("/home/ss/Desktop/Test/novi_fajl.bin", O_RDONLY);
  // result = lseek(fd, sizeof(char)*8, SEEK_SET);
  // result = write(fd, "", 1);
  map = (char*)mmap(0, 8*sizeof(char), PROT_READ , MAP_SHARED, fd, 0);
  cout<<map[7]<<endl;

  munmap(map, 8*sizeof(char));
  close(fd);
  return 0;
}
*/

// pisanje u mmap
/*
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
using namespace std;


int main(){
  
  int i;
  int fd; 
  int result;
  char* map;

  fd = open("/home/ss/Desktop/Test/novi_fajl.bin", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
  result = lseek(fd, sizeof(char)*8, SEEK_SET);
  result = write(fd, "", 1);
  map = (char*)mmap(0, 8*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  map[7] = 36;

  munmap(map, 8*sizeof(char));
  close(fd);
  return 0;
}
*/

// citanje iz binarnog fajla
/*
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(){

    string x = "";
    int y = 15;
    ifstream outfile;
    outfile.open("me.dat", ios::binary | ios::in);
    char c = 'a';
    while(true){
        outfile.read(&c, 1);
        if (c=='\0'){
            break;
        }
        x.push_back(c);
    }
    outfile.read((char*)&y, sizeof(int));
    cout<<x<<endl<<y<<endl;

    return 0;
}
*/

// pisanje u binarni fajl
/*

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

int main(){

    string x = "to sam samo ja";
    int y = 15;
    ofstream outfile;
    outfile.open("me.dat", ios::binary | ios::out);
    //outfile.write(x.c_str(), x.size());
    //outfile.write((char*)&x, sizeof(y));
    char file[x.length()+1];
    strcpy(file, x.c_str());
    outfile.write(file, sizeof(file));
    outfile.write((char*)&x, sizeof(int));

    return 0;
}
*/


