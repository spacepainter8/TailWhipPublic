%{

  #include "stdio.h"
  #include "../inc/parserHelper.hpp"
  #include "../inc/assembler.hpp"
  #include "../inc/common.hpp"
  #include <iostream>
  #include <stdio.h>
  using namespace std;
  extern int yylex(void);
  extern void yyerror (const char*);
  extern int lineNum;
  extern int yyparse();
  extern FILE* yyin;


%}


//%output "parser.cpp"
//%defines "parser.hpp"



%union {
  char* label;
  int literal;
}

%token ENDL;
%token COMMENT;
%token GLOBAL;
%token EXTERN;
%token SECTION;
%token WORD;
%token SKIP;
%token END;

%token HALT;
%token INT;
%token IRET;
%token CALL;
%token RET;
%token JMP;
%token BEQ;
%token BNE;
%token BGT;
%token PUSH;
%token POP;
%token XCHG;
%token ADD;
%token SUB;
%token MUL;
%token DIV;
%token NOT;
%token AND;
%token OR;
%token XOR;
%token SHL;
%token SHR;
%token LD;
%token ST;
%token CSRRD;
%token CSRWR;

%token DEC_LITERAL;
%token HEX_LITERAL;
%token BIN_LITERAL;
%token OCT_LITERAL;
%token COMMA;
%token DOLLAR;
%token LBRACKET;
%token RBRACKET;
%token PLUS;
%token REGISTER;
%token CSREGISTER;

%token <label> SYMBOL;
%token <label> LABEL;



%type <label> symbols;
%type <literal> literalRule;
%type <literal> REGISTER;
%type <label> CSREGISTER;


%%
code:
| line code;

line:
ENDL {;}
| codeline ENDL {
  parserHelper::fixLineNum(lineNum);
  assm->firstPass(lineNum-1);
}
| COMMENT ENDL {;}
| codeline COMMENT ENDL {
  assm->firstPass(lineNum-1);
}
| END {
  parserHelper::addCodeline(Types::directive,"end", lineNum);
  assm->firstPass(lineNum);
  YYACCEPT;
} 

codeline:
LABEL {
  parserHelper::addCodeline(Types::label,$1, lineNum);

  free($1);
}
| command
| LABEL command {
  parserHelper::addCodeline(Types::label,$1, lineNum);
  free($1);
}

command:
GLOBAL {
    parserHelper::addCodeline(Types::directive,"global", lineNum);
} symbols
| EXTERN{
  parserHelper::addCodeline(Types::directive,"extern", lineNum);
} symbols
| SECTION SYMBOL {
  parserHelper::addCodeline(Types::directive,"section", lineNum);
  parserHelper::addSArg(lineNum, $2);
  free($2);
}
| WORD {
  parserHelper::addCodeline(Types::directive,"word", lineNum);
} list
| SKIP literalRule {
  parserHelper::addCodeline(Types::directive,"skip", lineNum);
  parserHelper::addIArg(lineNum, $2);
}
| HALT {
  parserHelper::addCodeline(Types::command,"halt", lineNum);
}
| INT {
  parserHelper::addCodeline(Types::command,"int", lineNum);
}
| IRET {
  parserHelper::addCodeline(Types::command,"iret", lineNum);
}
| CALL literalRule {
  parserHelper::addCodeline(Types::command,"call", lineNum);
  parserHelper::addIArg(lineNum, $2);
}
| CALL SYMBOL {
  parserHelper::addCodeline(Types::command,"call", lineNum);
  parserHelper::addSArg(lineNum, $2);
  free($2);
}
| RET {
  parserHelper::addCodeline(Types::command,"ret", lineNum);
}
| JMP literalRule {
  parserHelper::addCodeline(Types::command,"jmp", lineNum);
  parserHelper::addIArg(lineNum, $2);
}
| JMP SYMBOL {
  parserHelper::addCodeline(Types::command,"jmp", lineNum);
  parserHelper::addSArg(lineNum, $2);
  free($2);
}
| BEQ REGISTER COMMA REGISTER COMMA literalRule {
  parserHelper::addCodeline(Types::command,"beq", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
  parserHelper::addIArg(lineNum, $6);
}
| BEQ REGISTER COMMA REGISTER COMMA SYMBOL {
  parserHelper::addCodeline(Types::command,"beq", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
  parserHelper::addSArg(lineNum, $6);
  free($6);
}
| BNE REGISTER COMMA REGISTER COMMA literalRule {
  parserHelper::addCodeline(Types::command,"bne", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
  parserHelper::addIArg(lineNum, $6);
}
| BNE REGISTER COMMA REGISTER COMMA SYMBOL {
  parserHelper::addCodeline(Types::command,"bne", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
  parserHelper::addSArg(lineNum, $6);
  free($6);
}
| BGT REGISTER COMMA REGISTER COMMA literalRule {
  parserHelper::addCodeline(Types::command,"bgt", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
  parserHelper::addIArg(lineNum, $6);
}
| BGT REGISTER COMMA REGISTER COMMA SYMBOL {
  parserHelper::addCodeline(Types::command,"bgt", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
  parserHelper::addSArg(lineNum, $6);
  free($6);
}
| PUSH REGISTER {
  parserHelper::addCodeline(Types::command,"push", lineNum);
  parserHelper::addIArg(lineNum, $2);
}
| POP REGISTER {
  parserHelper::addCodeline(Types::command,"pop", lineNum);
  parserHelper::addIArg(lineNum, $2);
}
| XCHG REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"xchg", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| ADD REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"add", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| SUB REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"sub", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| MUL REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"mul", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| DIV REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"div", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| NOT REGISTER {
  parserHelper::addCodeline(Types::command,"not", lineNum);
  parserHelper::addIArg(lineNum, $2);
}
| AND REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"and", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| OR REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"or", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| XOR REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"xor", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| SHL REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"shl", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| SHR REGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"shr", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| LD {
  parserHelper::addCodeline(Types::command,"ld", lineNum);
} operand COMMA REGISTER {
  parserHelper::addIArg(lineNum, $5);
}
| ST {
  parserHelper::addCodeline(Types::command,"st", lineNum);
} REGISTER {
  parserHelper::addIArg(lineNum, $3);
} COMMA operand
| CSRRD CSREGISTER COMMA REGISTER {
  parserHelper::addCodeline(Types::command,"csrrd", lineNum);
  parserHelper::addSArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| CSRWR REGISTER COMMA CSREGISTER {
  parserHelper::addCodeline(Types::command,"csrwr", lineNum);
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addSArg(lineNum, $4);
}

operand:
DOLLAR literalRule {
  parserHelper::addSArg(lineNum, "$");
  parserHelper::addIArg(lineNum, $2);
}
| DOLLAR SYMBOL {
  parserHelper::addSArg(lineNum, "$");
  parserHelper::addSArg(lineNum, $2);
  free($2);
}
| literalRule {
  parserHelper::addIArg(lineNum, $1);
} 
| SYMBOL {
  parserHelper::addSArg(lineNum, $1);
  free($1);
}
| REGISTER {
  parserHelper::addSArg(lineNum, "%");
  parserHelper::addIArg(lineNum, $1);
}
| LBRACKET REGISTER RBRACKET {
  parserHelper::addSArg(lineNum, "[]");
  parserHelper::addIArg(lineNum, $2);
}
| LBRACKET REGISTER PLUS literalRule RBRACKET {
  parserHelper::addSArg(lineNum, "[]");
  parserHelper::addIArg(lineNum, $2);
  parserHelper::addIArg(lineNum, $4);
}
| LBRACKET REGISTER PLUS SYMBOL RBRACKET {
  parserHelper::addSArg(lineNum, "[]");
  parserHelper::addSArg(lineNum, $4);
  free($4);
  parserHelper::addIArg(lineNum, $2);
}

list:
SYMBOL {
  parserHelper::addSArg(lineNum, $1);
  free($1);
}
| literalRule {
  parserHelper::addIArg(lineNum, $1);
}
| list COMMA SYMBOL {
  parserHelper::addSArg(lineNum, $3);
  free($3);
}
| list COMMA literalRule{
  parserHelper::addIArg(lineNum, $3);
}


symbols:
SYMBOL {
  parserHelper::addSArg(lineNum, $1);
  free($1);
  //cout<<$1<<endl;
}
|symbols COMMA SYMBOL {
  parserHelper::addSArg(lineNum, $3);
  free($3);
  //cout<<$3<<endl;
}



literalRule: 
DEC_LITERAL {;} 
| HEX_LITERAL {;} 
| BIN_LITERAL {;} 
| OCT_LITERAL {;}
%%



void yyerror(const char* error){
    //printf("Parser error on line %d", lineNum);
    //printf("\n%s", error);
    cout<<"Parser error on line"<<lineNum;
    cout<<endl<<error;
    exit(-1);
}

