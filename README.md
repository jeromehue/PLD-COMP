# C compiler with Antlr4/C++ (H4244)
This is a long term project in 4th year in computer science at INSA Lyon.<br/>
The goal is to create a compiler for a subset of the C programming language.<br/>

https://github.com/jeromehue/PLD-COMP

`grammar/` contains the Antlr4 grammar for a subset of C (.g4 files)<br/>
`src/` contains the source code (.cpp and .h files)<br/>
`tests/` contains everything related to tests

## Prerequisites
Make sure to create a `Makefile.local` file containing your local Antlr4 paths, e.g.:
```Makefile
ANTLR4_BINDIR=/usr/bin
ANTLR4_INCDIR=/shares/public/tp/ANTLR4-CPP/antlr4-runtime
ANTLR4_LIBDIR=/shares/public/tp/ANTLR4-CPP/lib
ANTLR4_RUNTIME=libantlr4-runtime.a # or libantlr4-runtime.so
```
The above example should work with computers of IF rooms 208 and 219.

## Usage
Compile the project by running `make`.<br/>
Execute our compiler on `example.c` by running `./bin/ifcc example.c`.<br/>
The assembly produced will go into `output.s` file.  

You can use `-v` for a verbose output.

## Tests
Run tests with `make test`.<br/>
Detailed test results will go into `tests/out/`.   
As the time of writing, the 'codeblocks' and 'unary-operators' tests fail,  
because they cover unimplemented features.
