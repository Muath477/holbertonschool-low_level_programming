# Simple Calculator

A terminal-based calculator written in C.

## Compilation

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 calculator.c -o calculator

## How to Run

./calculator

The program shows a menu, then asks for a choice in a loop.
Type the number of the operation you want and press Enter.
For operations 1 to 4, the program asks for two operands (A and B).
Type 0 to quit.

## Supported Operations

1) Add
2) Subtract
3) Multiply
4) Divide
0) Quit

## Numeric Behavior

The menu choice is read as an integer with scanf using %d.

The operands are read as decimals (double) with scanf using %lf, so the
calculator works with decimal numbers, not only integers.

Results are printed with %g. This means whole results print without extra
zeros (6 + 7 gives 13, not 13.000000), while fractional results keep their
decimals (10 / 4 gives 2.5).

Division by zero is checked before the division is performed. If B is zero
for the divide operation, the program prints an error and returns to the menu
instead of dividing.

## Known Limitations

The menu is printed only once at the start, not before every choice.

If the user enters non-numeric input (for example a letter), scanf fails to
parse it. The program discards the rest of the input line and prints
"Invalid choice", then asks again. Without discarding the line, the program
would loop forever on the same bad input.
