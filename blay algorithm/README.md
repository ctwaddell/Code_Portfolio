YEAR: Fall 2023 - Ongoing
SUMMARY:
Blay is an independent project for use in a video game I am developing. The puzzle generation algorithms are written here.

In summary, the game has a board constructed of a grid of arbitrary tiles. The tiles can share eachother as neighbors, but not every cell in the grid has to be a tile. Upon the tiles, pieces (called "Blays") are placed. Each Blay has a color and height. Given the following two conditions:
- Adjacent Blays cannot share the same color
- Adjacent Blays may not have a height difference greater than 1
uniquely solvable (only one solution) puzzles can be generated.

Based on the aforementioned rules, the algorithms construct a graph where the nodes represent a given Blay, and an edge represents the unswappability of the Blays. In other words, an edge means that swapping the Blays would lead to a contradiction of the rules.
Once the graph is created, a specified number of Blays will be (attempted) to be removed. To do this, the Bronn-Kerbosch algorithm is used to find the largest clique in the graph, which represents a group of Blays that can all be removed while ensuring a unique solution.
However, the current algorithm has flaws that periodically result in a non-unique solution. Because of this, a solver algorithm is run afterwards to ensure it is unique before saving it. If this fails, a new puzzle is generated.


TO USE:

First, run make
There will be various outputs, each section of this covers how to use one if the instructions provided by them don't make sense

./generator
This one you enter the dimensions of your board, and then enter your board as 0s and 1s. This will generate a valid board setup and output the string for it.

./solver
This one you have to give it a valid PUZZLE string (something like, 2,2,B1,V0,O2,E0|Y3... something where it has the |). if you give it a regular string it wont do anything. Once you give it the valid puzzle string, it will brute force solve the puzzle, and tell you how many solutions it found for the given puzzle string.

./tester
This one is just some stuff to play around and manipulate the board you give it. Primarily used for testing. The commands are:
a - Add a blay to a tile
r - Remove a blay from a tile
n - Create a new blay
d - Delete a blay
p - print the list of free blays (kind of redundant? I forgot)
f - output the format string of the board and freeblays
s - swap two blays
m - output the adjacency matrix if you want that for some reason
k - run the kerbosch algorithm which finds the max clique size in theory for the current board state (you probably need a full board for this one to mean anything)
c - complete/solve the current board with the given freeblays. It will output the solution
ENTER - will just print out the info of the board.This is also printed on each command.

./batch
This one is used to actually generate complete unique puzzles. You enter the board and then the amount of blays you want to remove. Generally I've found 12 or 13 is the current limit for computability within a decent timespan (obviously dependent on the board layout, but worst case scenario is usually a few minutes). The in progress output is in output.txt, and if it finds a valid solution it will append the board to finalpuzzles.txt. I am 99.99% sure these are unique and valid puzzles. Just short of a logical proof.

./automatic
This one automatically generates a puzzle 