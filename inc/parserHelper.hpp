
#ifndef PARSERHELPER_H
#define PARSERHELPER_H




#include <string>
#include <vector>
using namespace std;

enum Types {
    label = 0,
    directive = 1,
    command = 2
};

struct Arg {
  string argS;
  int argI;
};

struct Line {
  Types type;
  string name;
  int lineNum;
  vector<Arg> args;
};


class parserHelper {
  // napraviti ovo da bude singleton
   
public:
  static vector<Line> code;
  static void addCodeline(Types, string, int);
  static void fixLineNum(int);
  static void test();
  static void addSArg(int, string);
  static void addIArg(int, int);
  static void addSIArg(int, string, int);
};



#endif






