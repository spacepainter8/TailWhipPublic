#ifndef STE_HPP
#define STE_HPP

#include <string>
using namespace std;


static int INDEX=0;


enum Type {
  SCTN,
  NOTYP
};

enum Bind {
  LOC,
  GLOB
};


class SymbolTableEntry {
  private:
    int index = INDEX++;
    uint value;
    Type type;
    Bind bind;
    // sectionIndex = -1 if .global but not defined in current section
    // sectionIndex = -2 if .extern 
    int sectionIndex;
    string name;
    int fileNum = 0;

  public:
    int getIndex() const {
        return index;
    }

    uint getValue() const {
        return value;
    }

    void setValue(uint newValue) {
        value = newValue;
    }

    Type getType() const {
        return type;
    }

    void setType(Type newType) {
        type = newType;
    }

    Bind getBind() const {
        return bind;
    }

    void setBind(Bind newBind) {
        bind = newBind;
    }

    int getSectionIndex() const {
        return sectionIndex;
    }

    void setSectionIndex(int newSectionIndex) {
        sectionIndex = newSectionIndex;
    }

    string getName() const {
        return name;
    }

    void setName(const string& newName) {
        name = newName;
    }

    void setIndex(int newIndex){
        this->index = newIndex;
    }
    
    int getFileNum() const{
        return this->fileNum;
    }

    void setFileNum(int newFileNum){
        this->fileNum = newFileNum;
    }

    friend ostream& operator<< (ostream& os, const SymbolTableEntry& s);
};









#endif