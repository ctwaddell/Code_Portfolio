#include "blay.h"
#include "blaylist.h"
#include "tile.h"
#include "tileset.h"
#include "board.h"

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

int main(int argc, char** argv)
{
    std::string input;

    int rows;
    int columns;

    while(1)
    {
        std::cout << "How tall of a board?\n";
        std::getline(std::cin, input);
        if((rows = stoi(input)) == 0 || rows < 0) continue;
        break;
    }

    while(1)
    {
        std::cout << "How wide of a board?\n";
        std::getline(std::cin, input);
        if((columns = stoi(input)) == 0 || columns < 0) continue;
        break;
    }

    std::cout << "Enter the board shape:\n";

    char** characterBoard = new char*[rows];
    for(int r = 0; r < rows; r++)
    {
        characterBoard[r] = new char[columns];
    }

    for(int r = 0; r < rows; r++)
    {
        std::getline(std::cin, input);
        for(int c = 0; c < columns; c++)
        {
            if(input[c] != '1' && input[c] != '0') characterBoard[r][c] = '0';
            else if (input[c] == '1') characterBoard[r][c] = '1';
            else input[c] = '0';
        }
    }

    Board tileBoard(rows, columns);
    tileBoard.ParseBoard(characterBoard);
    tileBoard.ParseNeighbors();
    tileBoard.Generate();

    tileBoard.Print();
    std::cout << tileBoard.Format() << std::endl;
}