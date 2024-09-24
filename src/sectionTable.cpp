#include "../inc/sectionTable.hpp"
#include <iostream>
using namespace std;

ostream &operator<<(ostream &os, const SectionTableEntry& s){
  string index = "00000000";
  string length = "00000000";
  string base = "00000000";
  sprintf(&(index[0]), "%03X", s.getIndex());
  sprintf(&(length[0]), "%08X", s.getLength());
  sprintf(&(base[0]), "%08X", s.getBase());
  os<<s.getIndex()<<"\t"<<s.getName()<<"\t"<<"0x"<<length<<"\t"<<"0x"<<base<<endl;
  return os;
}