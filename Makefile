# Create this file locally and put your
# Antlr4 paths in it (see README.md)
include Makefile.local

# Compiler and arguments
CC := clang++
CCARGS := -g -c -I $(ANTLR4_INCDIR) -I src/grammar -std=c++17 -Wno-defaulted-function-deleted -Wno-unknown-warning-option
LDARGS := -g 

# Files
ANTLR_SOURCES := src/grammar/ifccBaseVisitor.cpp src/grammar/ifccLexer.cpp src/grammar/ifccParser.cpp src/grammar/ifccVisitor.cpp
SOURCES := $(ANTLR_SOURCES) $(wildcard src/*.cpp)
HEADERS := $(wildcard src/*.h)
OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:src/%=build/%)
GRAMMAR := grammar/ifcc.g4

# Default target
all: ifcc

# .o => ifcc
ifcc: $(ANTLR_SOURCES) $(HEADERS) $(OBJECTS)
	@echo "Édition des liens"
	@$(CC) $(LDARGS) $(OBJECTS) $(ANTLR4_LIBDIR)/$(ANTLR4_RUNTIME) -o ifcc

# .cpp => .o
build/%.o: src/%.cpp
	@echo "Compilation de $<"
	@mkdir -p build/grammar/
	@$(CC) $(CCARGS) $< -o $@

# .g4 => .cpp
$(ANTLR_SOURCES): $(GRAMMAR)
	@echo "Génération du code Antlr4"
	@$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o src $(GRAMMAR)

test: ifcc
	@cd tests && ./test.sh

maintest: ifcc
	@./ifcc maintest.c &>/dev/null 
	@echo 'Output produced : '
	@cat output.s
	gcc output.s && ./a.out; echo $$?

clean:
	@rm -rf build/
	@rm -rf src/grammar/
	@rm -rf tests/out
	@rm -rf output.s a.out
# Old folders (keep this for now):
	@rm -rf src/generated/
	@rm -rf src/out/

.PHONY: all clean test maintest