#ifndef LTE_HPP
#define LTE_HPP

#include <vector>
#include <string>
using namespace std;

class LiteralTableEntry {
  private:
    int value;
    int location;
    // in case it is a label entry
    string name;

  public:
    int getValue() const {
        return value;
    }

    void setValue(int newValue) {
        value = newValue;
    }

    int getLocation() const {
        return location;
    }

    void setLocation(int newLocation) {
        location = newLocation;
    }

    string getName() const {
        return name;
    }

    void setName(string newName){
        name = newName;
    }

    friend ostream& operator << (ostream& os, const LiteralTableEntry& l);
};






#endif