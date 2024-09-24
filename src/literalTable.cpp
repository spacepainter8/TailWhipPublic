#include "../inc/literalTable.hpp"
#include <iostream>
using namespace std;


ostream &operator<<(ostream &os, const LiteralTableEntry &l){
  string value = "00000000";
  string location = "00000000";
  sprintf(&(value[0]), "%08X", l.getValue());
  sprintf(&(location[0]), "%08X", l.getLocation());
             
  os<<"0x"<<value<<"\t"<<"0x"<<location<<"\t";
  if (l.getName()!="") os<<l.getName();
  os<<endl;
  return os;
}
