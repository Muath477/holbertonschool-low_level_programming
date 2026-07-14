# Simple Calculator

A terminal-based calculator written in C.

## Compilation

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 calculator.c -o calculator

## How to Run

./calculator

The program shows a menu. Type the number of the operation you want and
press Enter. Type 0 to quit.

## Supported Operations

1) Add
2) Subtract
3) Multiply
4) Divide
0) Quit

## Numeric Behavior

The menu option is read as an integer using scanf with %d.

## Known Limitations

The program only validates that the menu option is an integer inside the
valid range. It does not handle non-numeric input such as letters or words.
If the user types something like "x" or "hello", the program may behave
unexpectedly. This is outside the scope of the mandatory part.
