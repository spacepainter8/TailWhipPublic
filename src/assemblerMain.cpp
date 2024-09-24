#include "../inc/parser.hpp"
#include "../inc/lexer.hpp"
#include "../inc/parserHelper.hpp"
#include "../inc/assembler.hpp"
#include "../inc/common.hpp"
#include "../inc/linker.hpp"
#include <stdio.h>
#include <iostream>
using namespace std;
#include <string>

Assembler* assm = new Assembler();

bool isPlaceOption (char* option){
  if (option[0]=='-' && option[1]=='p' && option[2]=='l' &&
  option[3]=='a' && option[4]=='c' && option[5]=='e' && option[6]=='=' ) return true;
  else return false;
}


int main (int argc, char* argv[]){
 
  if ( strcmp(argv[0], "./assembler")==0){
    // promeni ovo na odbrani
    

    // ovo mozda mora da se podesi
    string input = "/home/ss/Desktop/Projekat/tests/";
    if (argc==4 && strcmp(argv[1], "-o")==0){
      string outputFile = argv[2];
      input+=argv[3];
      assm->setOutputFile(outputFile);
    }
    else if (argc==2){
      string outputFile = "output.o";
      input+=argv[1];
      assm->setOutputFile(outputFile);
    }
    else {
      cout<<"Incorrect arguments for assembler!!"<<endl;
      exit(-1);
    }
    
    //input+=argv[3];
    //char* file = "/home/ss/Desktop/Projekat/tests/test.s";
    char file[input.length()+1];
    strcpy(file, input.c_str());
    FILE *myFile = fopen(file, "r");
    if (!myFile){
        //printf("Unable to open file!!\n");
        cout<<"Unable to open file!!"<<endl;
        return -1;
    }
  
    yyin = myFile;
    //yydebug = 1;
    do {
      yyparse();
    } while (!feof(yyin));

    //parserHelper::test();
    assm->secondPass();
    
    fclose(myFile);
  }
/*
  else if (strcmp(argv[0], "./linker")==0){
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
*/


    

    return 0;

}