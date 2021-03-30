# Create this file locally and put your
# Antlr4 paths in it (see README.md)
include Makefile.local

# Compiler and arguments
CC := clang++
CCARGS := -g -c -I $(ANTLR4_INCDIR) -I src/generated/grammar -std=c++17 -Wno-defaulted-function-deleted -Wno-unknown-warning-option
LDARGS := -g 

# Files
ANTLR_SOURCES := src/generated/grammar/ifccBaseVisitor.cpp src/generated/grammar/ifccLexer.cpp src/generated/grammar/ifccParser.cpp src/generated/grammar/ifccVisitor.cpp
SOURCES := $(shell find src -maxdepth 1 -name '*.cpp') $(ANTLR_SOURCES)
HEADERS := $(shell find src -maxdepth 1 -name '*.h')
OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:src/%=build/%)
GRAMMAR := grammar/ifcc.g4

# Default target
all: ifcc

# .o => ifcc
ifcc: $(ANTLR_SOURCES) $(HEADERS) $(OBJECTS)
	$(CC) $(LDARGS) $(OBJECTS) $(ANTLR4_LIBDIR)/$(ANTLR4_RUNTIME) -o ifcc

# .cpp => .o
build/%.o: src/%.cpp
	@mkdir -p build/generated/grammar
	$(CC) $(CCARGS) $< -o $@

# .g4 => .cpp
$(ANTLR_SOURCES): $(GRAMMAR)
	$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o src/generated $(GRAMMAR)

test:
	@cd tests && ./test.sh

maintest: 
	@./ifcc maintest.c &>/dev/null 
	@echo 'Output produced : '
	@cat output.s
	gcc output.s && ./a.out; echo $$?

clean:
	rm -rf build/
	rm -rf src/generated/
	rm -rf src/out/

.PHONY: all clean test maintest