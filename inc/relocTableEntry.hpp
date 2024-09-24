#ifndef RTE_HPP
#define RTE_HPP

#include <iostream>
using namespace std;

enum RelocType {
    R_32
};

class RelocTableEntry {
  private:
   int offset;
   RelocType type;
   int symbolIndex;
   int addend;

  public:
    int getOffset() const {
        return offset;
    }
  
   void setOffset(int newOffset) {
        offset = newOffset;
    }

    RelocType getType() const {
        return type;
    }

    void setType(RelocType newType) {
        type = newType;
    }

    int getSymbolIndex() const {
        return symbolIndex;
    }

    void setSymbolIndex(int newSymbolIndex) {
        symbolIndex = newSymbolIndex;
    }

    int getAddend() const {
        return addend;
    }

    void setAddend(int newAddend) {
        addend = newAddend;
    }

    friend ostream& operator<< (ostream& os, const RelocTableEntry& r);
};


#endif