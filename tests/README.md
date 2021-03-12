# C Compiler test framework

This directory contains scripts to test your compiler along with 
a collection of C language source codes that can be tested.

## Prerequisites

Docker must be installed in order to execute these scripts. The compiler has 
to be present in the directory `../compiler/ifcc`. Compiler options and location
can be customized in the `wrapper.sh` script.

## Files

`test.sh` is the base file to launch. It will execute the `test.py`
script. Results are put in a new directory `out`.

The tests are in the `tests` subfolder. 
