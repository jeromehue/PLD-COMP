# Create this file locally and put your 3 Antlr paths in it.
include Makefile.local

SRC=src
BUILD=build
OUTPUT=$(SRC)/out
GENERATED=$(SRC)/generated
GRAMMAR=grammar/ifcc.g4

SOURCES  := $(wildcard $(SRCDIR)/*.c)

CC=clang++
CCARGS=-g -c -I $(ANTLR4_INCDIR) -I $(GENERATED) -std=c++17 -Wno-defaulted-function-deleted -Wno-unknown-warning-option
LDARGS=-g

all: ifcc

ifcc: dirs antlr $(SOURCES) $(SRC)/visitor.h
	$(CC) $(CCARGS) $(SRC)/main.cpp  -o $(OUTPUT)/main.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccBaseVisitor.cpp -o $(OUTPUT)/ifccBaseVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccLexer.cpp -o $(OUTPUT)/ifccLexer.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccVisitor.cpp -o $(OUTPUT)/ifccVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccParser.cpp -o $(OUTPUT)/ifccParser.o 
	$(CC) $(CCARGS) $(SRC)/visitor.cpp -o $(OUTPUT)/visitor.o 
	$(CC) $(CCARGS) $(SRC)/cgen.cpp -o $(OUTPUT)/cgen.o 
	$(CC) $(LDARGS) $(OUTPUT)/main.o $(OUTPUT)/ifccBaseVisitor.o $(OUTPUT)/ifccLexer.o $(OUTPUT)/ifccVisitor.o $(OUTPUT)/ifccParser.o $(OUTPUT)/visitor.o $(OUTPUT)/cgen.o $(ANTLR4_LIBDIR)/$(ANTLR4_RUNTIME) -o ifcc

antlr: $(GRAMMAR)
	$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR)

dirs:
	mkdir -p $(OUTPUT) 
	mkdir -p $(GENERATED) 

test:
	@cd tests && ./test.sh

clean:
	rm -rf $(OUTPUT)
	rm -rf $(GENERATED)