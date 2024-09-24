%{
  #include "../inc/parser.hpp"
  #include "../inc/assembler.hpp"
  #include <string>
  using namespace std;
  #include <iostream>
  //#include "stdio.h"
  int lineNum=1;
%}

%option noyywrap


%%
, {return COMMA;}
\n {++lineNum; return ENDL;} 
[ \t\r]+ {;}
#[^\n\r]* {
  return COMMENT;};
[^ \n\r:,]*[ \t]*: {
    int n=0;
    for (int i=yyleng-1;i>=0;i--){
      if (yytext[i]==' ' || yytext[i]=='  ') n++;
      if (yytext[i]==':') break;
    }
    yylval.label = strndup(yytext, yyleng-n-1);
    return LABEL;
}
%sp {
  yylval.literal = 14;
  return REGISTER;
}
%pc {
  yylval.literal = 15;
  return REGISTER;
}
\${1} {
  return DOLLAR;
}
\[ {
  return LBRACKET;
}
\] {
  return RBRACKET;
}
\+ {
  return PLUS;
}
%r[0-9] {
  yylval.literal = stoi(yytext+2, nullptr, 10);
  return REGISTER;
}
%r1[0-5] {
  yylval.literal = stoi(yytext+2, nullptr, 10);
  return REGISTER;
}
%(status|handler|cause) {
  yylval.label = strdup(yytext+1);
  return CSREGISTER;
}
\.global {
  return GLOBAL;
}
\.extern {
  return EXTERN;
}
\.section {
  return SECTION;
}
\.word {
  return WORD;
}
\.skip {
  return SKIP;
}
\.end {
  return END;
}
halt {
  return HALT;
}
int {
  return INT;
}
iret {
  return IRET;
}
call {
  return CALL;
}
ret {
  return RET;
}
jmp {
  return JMP;
}
beq {
  return BEQ;
}
bne {
  return BNE;
}
bgt {
  return BGT;
}
push {
  return PUSH;
}
pop {
  return POP;
}
xchg {
  return XCHG;
}
add {
  return ADD;
}
sub {
  return SUB;
}
mul {
  return MUL;
}
div {
  return DIV;
}
not {
  return NOT;
}
and {
  return AND;
}
or {
  return OR;
}
xor {
  return XOR;
}
shl {
  return SHL;
}
shr {
  return SHR;
}
ld {

  return LD;
}
st {
  return ST;
}
csrrd {
  return CSRRD;
}
csrwr {
  return CSRWR;
}

0[0-7]+ {
  yylval.literal = (int)stol(yytext+1, nullptr, 8);
  return OCT_LITERAL;
}
-{0,1}[0-9]* {
  yylval.literal = (int)stol(yytext, nullptr, 10);
  return DEC_LITERAL;
}
0[xX][0-9a-fA-F]+ {
  
  yylval.literal = (int)stol(yytext+2, nullptr, 16);
  return HEX_LITERAL;
}
0b[01]+ {
  yylval.literal = (int) stol(yytext+2, nullptr, 2);
  return BIN_LITERAL;
}
[^ \n\r\t,$\[\]\+:]+ {
  yylval.label = strdup(yytext);
  return SYMBOL;
}



%%

