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
ANTLR4_INCDIR=/usr/include/antlr4-runtime
ANTLR4_LIBDIR=/usr/lib/x86_64-linux-gnu
ANTLR4_RUNTIME=libantlr4-runtime.a # or libantlr4-runtime.so
```

## Execution
Compile the project with `make`.<br/>
Run `./ifcc main.c` to run our compiler on `main.c`.<br/>
Note: You can get a versbose output with `-v`.

## Tests
Run tests with `make test`.<br/>
Detailed test results will go into `tests/out/`.
