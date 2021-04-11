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
The assembly output will go into `output.s`.  

You can use `-v` for a verbose output.

## Tests
Run tests with `make test`.<br/>
Detailed test results will go into `tests/out/`.

As the time of writing, the **codeblocks** and **unary-operators** tests fail, because they cover unimplemented features.

You can run a 'graphical' test to visualize the derivation tree produced by antlr.
* Download antlr complete java tool from https://www.antlr.org/download.html, place it under `tests/gui`.  
* Edit `launch.sh` to specify the CLASSPAT, for instance with antlr-4-9-complete.jar: 
  ```sh
  export CLASSPATH=".:./antlr-4.9-complete.jar:$CLASSPATH" 
  alias antlr4="java -jar ./antlr-4.9-complete.jar"
  ```
* Run launch.sh   

You can also modify the grammar and .c file used. By default, it uses `main.c` as source code and `ifcc.g4` as grammar, but the 
'official' C grammar is also available in `tests/gui`.

![antlr_parse](https://user-images.githubusercontent.com/57839770/114313738-ff8d9300-9af7-11eb-89dd-64a34ab3b5b3.png)
