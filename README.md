# C compiler with antlr4/c++
H4424 - Projet longue durée de 4IF à l'INSA Lyon.

## Folders
- `compiler` : the source code
- `tests` : the tests

## Important files
- `ifcc.g4` contains the grammar in antlr4 format
- `main.cpp` contains the C++ code to call the antlr4-generated parser on the  file name provided in the command line.
- `visitor.h` is a visitor of the parse tree that produces an assembly-language output

## Compilation scripts
- `Makefile` can be used to compile the parser. Libraries and include directories default to the values that work in the IF machines of rooms 208 and 219. You must create a `Makefile.local` file with your own Antlr4 paths.
- `compile_ubuntu.sh` is a script that compiles the project in a different environment (a ubuntu where the antlr runtime had to be compiled manually).
- `compile_docker.sh` is a script to use if you are desperate. It compiles the project in a docker environment where antlr4 and its dependencies are properly installed. 

## Tests
This directory contains scripts to test your compiler along with 
a collection of C language source codes that can be tested.

### Prerequisites
Docker must be installed in order to execute these scripts. The compiler has 
to be present in the directory `../compiler/ifcc`. Compiler options and location
can be customized in the `wrapper.sh` script.

### Running tests
`test.sh` is the base file to launch. It will execute the `test.py`
script. Results are put in a new directory `out`.

The tests are in the `tests` subfolder. 
