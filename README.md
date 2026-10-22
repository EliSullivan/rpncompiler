# rpncompiler

This program reads functions written in RPN and generates assembly code to 
perform the operations. It assumes the provided function name is valid in
32bit assembly and that provided numbers fit in 32 bits. The program separates
each line of input into tokens delimited by spaces or tabs, then performs a
different action for numbers, variables, and operators. The supported operators
are +, -, *, /, and variables can be any letter of the modern Latin alphabet.
Numbers are expected to be integers. Variables from the expression (which
become parameters for the function) are numbered in order of appearance. A
separate assembly source code file is made for each function.

Error handling
--------------

If something is invalid about the function,
the program writes an error to the console and deletes the assembly file.
Because of this, if there are multiple functions of the same name and the
second one is invalid but the first is valid, no function will be written since
the first one was already overwritten by the second before the second was
deleted.

Test cases
----------
test1.txt: The two provided test cases (f and g) as well as subtraction

test2.txt: Tests division with negative numbers, subtraction where there is
a negative number, numbers with many digits, and functions that contain no
operators.

test3.txt: Tests many values being pushed before an operator, odd formatting
like repeated spaces and a blank line, and a nested structure.

testbad.txt: Provides input that is invalid for a variety of reasons such as
containing an operator with not enough operands, containing an empty function,
not containing a colon after the function name, and numbers containing invalid
characters. The last function "ok" is an example of a function that should not
be rejected.

Any other test case can be provided by making a text file in the format of
function name : expression and running parser with the file as the argument e.g.
"./parser test1000.txt".

Building
--------

make or make all: compiles parser and builds all the assembly code for all the
test cases. This includes testbad, so errors are expected to be written to the
console when the invalid functions fail to be written.

make parser: only compiles the parser

make test1, test2, test3, or testbad: compiles parser and builds assembly code
for the specified test case.

make clean: removes executable and assembly source code files.