#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;
#include <string.h>
#include "../inc/linker.hpp"

bool isPlaceOption (char* option){
  if (option[0]=='-' && option[1]=='p' && option[2]=='l' &&
  option[3]=='a' && option[4]=='c' && option[5]=='e' && option[6]=='=' ) return true;
  else return false;
}



int main(int argc, char** argv){
  if (strcmp(argv[0], "./linker")==0){
    // linker
    // check when linker needs to start if the input vector is empty
    //<<argc<<endl;
    Linker linker = Linker();
    if (argc==1){
      cout<<"Must enter an input file for linker!!"<<endl;
      exit(-1);
    }
    for (int i=1;i<argc;i++){
      //cout<<argv[i]<<endl;
      if (strcmp(argv[i], "-o")==0){
        if (i==argc-1 || strcmp(argv[i+1], "-o")==0 | strcmp(argv[i+1], "-hex")==0 | isPlaceOption(argv[i+1])){
          cout<<"Invalid call of linker!!"<<endl;
          exit(-1);
        }
        linker.setOutputFile(argv[i+1]);
        i++;
      }
      else if (isPlaceOption(argv[i])){
        if (i==argc-1){
          cout<<"Invalid call of linker!!"<<endl;
          exit(-1);
        }
        char c = argv[i][7];
        string section;
        int cnt = 7;
        while(c!='@' && c!='\0'){
            section.push_back(c);
            cnt++;
            c = argv[i][cnt];
            
        }
        if (c=='0' || section=="") {cout<<"Error in -place option!!"<<endl;exit(-1);}
        string address;
        cnt++;
        c = argv[i][cnt];
        while(c!='\0'){
          address.push_back(c);
          cnt++;
          c = argv[i][cnt];
        }
        if (c=='0' || address=="") {cout<<"Error in -place option!!"<<endl;exit(-1);}
        //cout<<section<<endl<<address<<endl;
        linker.addPlaceOption(section, address.c_str());
        
      }
      else if (strcmp(argv[i], "-hex")==0){
        linker.setHexSet(true);
      }
      else {
        linker.addInputFile(argv[i]);
        i++;
        for (;i<argc;i++){
          
          if (strcmp(argv[i], "-o")==0 | strcmp(argv[i], "-hex")==0 | isPlaceOption(argv[i])){
            cout<<"Can't have options after input files!!"<<endl;
            exit(-1);
          }
           
          else {linker.addInputFile(argv[i]);}
          
        }
        
         
      }
      
    }
   
    linker.startLinking();
  }
  return 0;
}