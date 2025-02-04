YEAR: WINTER 2025
SUMMARY:
A collection of C++ programs to generate, manipulate, and verify uniquely solvable puzzles for my upcoming Unity solo game project, Blay. Creates a suite of programs for these purposes, accomodating user input and parameters.

Makefile usage and targets:
    make all - creates all executables
    make auto/automatic - creates ./auto
    make manual - creates ./manual
    make website - creates ./website
    make generator - creates ./generator
    make tester - creates ./tester
    make dll/dll64 - creates RPG.dll which essentially ./auto in a dll for x86_64 systems (64 bit)
    make dll32 - creates RPG32.dll for x86 systems (32 bit)
    make clean - removes all related files and outputs

./generator - usage: ./generator
    Prompts manual input of board shape to generate a complete board pattern. First, specify row number, then column number, and then board shape aligning to the specified rows x columns grid. 0's indicate an empty space and 1's indicate a filled space.

./tester - usage: ./tester
    Prompts an input of a board string, either with a Blaylist or without. Allows for interaction with the board and other features:
    a - Add a Blay to a tile
    r - Remove a Blay from a tile
    n - Create a new Blay
    d - Delete a Blay
    p - Print the Blaylist
    f - Print the format string of the board and Blaylist
    s - Swap two Blays
    m - Print the adjacency matrix of the board
    k - Run the Bronn-Kerbosch algorithm which finds the max clique size for the current board state
    c - Complete/solve the current board with the given Blaylist. Print the solution(s)
    ENTER - Print out board info. Also printed each command
    h - Help/print a list of commands
    z - Create a puzzle up to input size
    x - Create a puzzle up to input size with debug info

./auto - usage: ./auto rows columns seed
    Generates a random puzzle constrained by input parameters. By default aims for a puzzle of up to size 10. Prints the board and the solution

./manual - usage: ./manual
    Similar to ./auto and ./generator. Prompts a board shape and then generates a random puzzle up to specified input size. Prints the board and the solution

./website - usage: ./website rows columns
    Similar to ./auto but is unseeded (seed is based on current time). Outputs a random puzzle (JSON formatted) of size rows x columns to dailyPuzzle.txt