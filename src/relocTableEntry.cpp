#include "../inc/relocTableEntry.hpp"

ostream &operator<<(ostream &os, const RelocTableEntry& r){
  string offset = "00000000";
  string index = "00000000";
  string addend = "00000000";
  sprintf(&(offset[0]), "%08X", r.getOffset());
  sprintf(&(index[0]), "%03X", r.getSymbolIndex());
  sprintf(&(addend[0]), "%08X", r.getAddend());
              
  os<<"0x"<<offset<<"\t"<<r.getSymbolIndex()<<"\t"<<"0x"<<addend<<"\t";
  if (r.getType()==RelocType::R_32){
    os<<"R_32";
  }
  
  return os;
}