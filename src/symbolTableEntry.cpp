#include "../inc/symbolTableEntry.hpp"
#include <iostream>
using namespace std;


ostream &operator<<(ostream &os, const SymbolTableEntry &s){
  string str = "00000000";
  sprintf(&(str[0]), "%03X", s.getIndex());
  os<<s.getIndex()<<"\t";
  str = "00000000";
  sprintf(&(str[0]), "%08X", s.getValue());
  os<<"0x"<<str<<"\t";
  //<<s.getType()<<"\t"<<s.getBind()<<"\t"<<s.getSectionIndex()<<"\t"<<s.getName()<<endl;
  if (s.getType()==Type::NOTYP) os<<"NOTYP";
  else os<<"SCTN";
  os<<"\t";
  if (s.getBind()==Bind::GLOB) os<<"GLOB";
  else os<<"LOC";
  os<<"\t";
  str = "00000000";
  sprintf(&(str[0]), "%03X", s.getSectionIndex());
  os<<s.getSectionIndex()<<"\t"<<s.getName()<<"\t"<<s.getFileNum()<<endl;
  return os;
}