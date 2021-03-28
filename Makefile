# Create this file locally and put your
# Antlr4 paths in it (see README.md)
include Makefile.local

SRC=src
BUILD=build
OUTPUT=$(SRC)/out
GENERATED=$(SRC)/generated
GRAMMAR=grammar/ifcc.g4

SOURCES := $(wildcard $(SRCDIR)/*.c)

CC=clang++
CCARGS=-g -c -I $(ANTLR4_INCDIR) -I $(GENERATED)/grammar -std=c++17 -Wno-defaulted-function-deleted -Wno-unknown-warning-option
LDARGS=-g

all: ifcc

ifcc: dirs antlr $(SOURCES) $(SRC)/visitor.h
	$(CC) $(CCARGS) $(SRC)/main.cpp -o $(OUTPUT)/main.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccBaseVisitor.cpp -o $(OUTPUT)/ifccBaseVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccLexer.cpp -o $(OUTPUT)/ifccLexer.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccVisitor.cpp -o $(OUTPUT)/ifccVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/grammar/ifccParser.cpp -o $(OUTPUT)/ifccParser.o 
	$(CC) $(CCARGS) $(SRC)/visitor.cpp -o $(OUTPUT)/visitor.o 
	$(CC) $(CCARGS) $(SRC)/visitor.cpp -o $(OUTPUT)/visitor.o 
	$(CC) $(CCARGS) $(SRC)/Program.cpp -o $(OUTPUT)/Program.o 
	$(CC) $(CCARGS) $(SRC)/IR.cpp -o $(OUTPUT)/IR.o 
	$(CC) $(LDARGS) $(OUTPUT)/main.o $(OUTPUT)/ifccBaseVisitor.o $(OUTPUT)/ifccLexer.o $(OUTPUT)/ifccVisitor.o $(OUTPUT)/ifccParser.o $(OUTPUT)/visitor.o $(OUTPUT)/Program.o $(OUTPUT)/IR.o $(ANTLR4_LIBDIR)/$(ANTLR4_RUNTIME) -o ifcc

antlr: $(GRAMMAR)
	$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR)

dirs:
	rm -rf $(OUPUT) 	; mkdir -p $(OUTPUT) 
	rm -rf $(GENERATED) ; mkdir -p $(GENERATED) 

test:
	@cd tests && ./test.sh

maintest : 
	@./ifcc maintest.c &>/dev/null 
	@echo 'Output produced : '
	@cat output.s
	gcc output.s && ./a.out; echo $$?

clean:
	rm -rf $(OUTPUT)
	rm -rf $(GENERATED)
