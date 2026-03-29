# Welcome to My Bc
***

## Description
The program receives a mathematical expression as a command line argument. It then parses the string into an 
output queue according to the shunting-yard algorithm and calculates the result using reverse polish notation. 
The mathematical expression may contain numbers as integers, operators +, -, *, /, %, round brackets and spaces. 
It does not handle expressions with unary minus. 

## Installation
You can install the program by running make.
As a prerequisite you need to have a GCC compiler and the Make utility.

## Usage
The user can enter the expression the following way:
```
./my_bc "expression"
```
If the expression does not contain any errors, the result of the expression will be provided as output.
Otherwise the program will encounter a parse error and output the corresponding message: "parse error".
Division by zero is not allowed and will cause the error: "divide by zero".
