ASS_SRC = ./src/assemblerMain.cpp ./src/parserHelper.cpp ./src/assembler.cpp ./src/literalTable.cpp ./src/sectionTable.cpp ./src/symbolTableEntry.cpp ./src/relocTableEntry.cpp
LINKER_SRC = ./src/linkerMain.cpp ./src/linker.cpp ./src/sectionTable.cpp ./src/symbolTableEntry.cpp ./src/relocTableEntry.cpp
EMULATOR_SRC = ./src/emulatorMain.cpp ./src/emulator.cpp 

assembler: parser.cpp lexer.cpp $(ASS_SRC)
	g++  -g  ./src/parser.cpp ./src/lexer.cpp $(ASS_SRC) -lfl -I ./inc/ -o assembler

linker: $(LINKER_SRC)
	g++  -g  $(LINKER_SRC) -lfl -I ./inc/ -o linker

emulator: $(EMULATOR_SRC)
	g++  -g $(EMULATOR_SRC) -lfl -I ./inc/ -o emulator


lexer.cpp: ./misc/lexer.l
	flex --header-file=./inc/lexer.hpp --outfile=./src/lexer.cpp ./misc/lexer.l  

parser.cpp: ./misc/parser.y ./misc/lexer.l 
	bison ./misc/parser.y --defines=./inc/parser.hpp --output=./src/parser.cpp

clean:
	rm -rf ./src/lexer.cpp ./inc/lexer.hpp ./src/parser.cpp ./inc/parser.hpp 
