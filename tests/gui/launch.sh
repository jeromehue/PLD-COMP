#!/bin/sh
export CLASSPATH=".:./antlr-4.9-complete.jar:$CLASSPATH"
alias antlr4="java -jar ./antlr-4.9-complete.jar"
alias grun="java org.antlr.v4.gui.TestRig"

# Generate parser and lexer using antlr
antlr4 ifcc.g4

# Compile
javac *.java

# lauch gui with main.c file
grun bin/ifcc prog -gui < main.c
