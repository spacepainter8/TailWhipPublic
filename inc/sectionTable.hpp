#ifndef SECT_HPP
#define SECT_HPP

#include <string>
using namespace std;

static int INDEXS = 0;

class SectionTableEntry {
  private:
    int index = INDEXS++;
    string name;
    uint base;
    uint length;

  public:
    string getName() const {
        return name;
    }

    void setName(const string& newName) {
        name = newName;
    }

    uint getBase() const {
        return base;
    }

    void setBase(uint newBase) {
        base = newBase;
    }

    uint getLength() const {
        return length;
    }

    void setLength(uint newLength) {
        length = newLength;
    }

    int getIndex() const {
        return index;
    }

    void setIndex(int newIndex){
        index = newIndex;
    }

    friend ostream& operator<< (ostream& os, const SectionTableEntry& s);
};





#endif