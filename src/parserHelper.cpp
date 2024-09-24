#include "../inc/parserHelper.hpp"
#include <iostream>
using namespace std;

vector<Line> parserHelper::code;

void parserHelper::addCodeline(Types type, string name, int lineNum){
  Line line;
  line.type=type;
  line.name=name;
  line.lineNum=lineNum;
  

  code.push_back(line);
}

void parserHelper::test(){

  for(Line l: parserHelper::code){
    int cnt = 0;
     cout<<l.name<<"("<<l.lineNum<<") - ";
     for (Arg arg: l.args){
       if (arg.argS!="") cout<<arg.argS<<'-'<<cnt++<<',';
       else cout<<arg.argI<<'-'<<cnt++<<",";
     }
    cout<<endl;
  }
}

void parserHelper::addSArg(int lineNum, string arg){
  for (Line& l: parserHelper::code){
    if (l.lineNum==lineNum && l.type!=Types::label){
        //l.args.push_back(arg);
        
        Arg a;
        a.argS=arg;
        l.args.push_back(a);
        return;
    }
  }
  
}

void parserHelper::addIArg(int lineNum, int arg){
  for (Line& l : parserHelper::code ){
    if (l.lineNum==lineNum && l.type!=Types::label){
      Arg a;
      a.argI = arg;
      l.args.push_back(a);
      return;
    }
  }
}

void parserHelper::addSIArg(int lineNum, string sArg, int iArg){
  for (Line& l : parserHelper::code){
    if (l.lineNum==lineNum){
      Arg a;
      a.argI = iArg;
      a.argS = sArg;
      l.args.push_back(a);
      return;
    }
  }
}

void parserHelper::fixLineNum (int lineNum){
  for (Line& l: parserHelper::code){
    if (l.lineNum==lineNum && l.type==Types::label) {l.lineNum--;return;}
  }
}