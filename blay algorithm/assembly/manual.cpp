#include "board.h"
#include "tile.h"
#include "blay.h"
#include "tileset.h"
#include "blaylist.h"
#include "adjmatrix.h"

#include <fstream>
#include <chrono>

int SmartRemoveCount(int puzzleSize)
{
    if(puzzleSize > 10) return 10;
    return (puzzleSize - ((puzzleSize % 2) + 1));
}

int main(int argc, char** argv)
{
    std::string input;
    int rows;
    int columns;
    
    while(1)
    {
        std::cout << "How wide of a board?" << std::endl;;
        std::getline(std::cin, input);
        if((columns = stoi(input)) == 0 || columns < 0) continue;
        break;
    }

    while(1)
    {
        std::cout << "How tall of a board?" << std::endl;
        std::getline(std::cin, input);
        if((rows = stoi(input)) == 0 || rows < 0) continue;
        break;
    }

    std::cout << "Enter the board shape:" << std::endl;

    char** characterBoard = new char*[rows];
    for(int r = 0; r < rows; r++)
    {
        characterBoard[r] = new char[columns];
    }

    int boardSize = 0;
    for(int r = 0; r < rows; r++)
    {
        std::getline(std::cin, input);
        for(int c = 0; c < columns; c++)
        {
            if(input[c] != '1' && input[c] != '0') characterBoard[r][c] = '0';
            else if (input[c] == '1') 
            {
                characterBoard[r][c] = '1';
                boardSize++;
            }
            else input[c] = '0';
        }
    }

    std::cout << "Enter desired puzzle size < 12:" << std::endl;
    std::getline(std::cin, input);

    int removeCount = std::stoi(input) == 0 ? SmartRemoveCount(boardSize) : stoi(input);

    int attempts = 0;
    int TOLERANCE = 100;
    std::vector<int> solutionIDs;
    std::string puzzleString;
    do
    {
        attempts++;
        if(attempts % TOLERANCE == 0) //will try TOLERANCE times to get a puzzle of size removeCount, and if fails, lowers threshhold
        {
            removeCount--;
        }

        solutionIDs.clear();
        Board tileBoard(rows, columns);
        tileBoard.ParseBoard(characterBoard);
        tileBoard.Generate();
        tileBoard.string = tileBoard.Format();
        puzzleString = tileBoard.Format();

        solutionIDs = tileBoard.FastGeneratePuzzle(removeCount);
    } while((int)solutionIDs.size() < removeCount);

    Board solutionBoard(puzzleString);

    std::cout << solutionBoard.Format() << std::endl;

    for(int i = 0; i < (int)solutionIDs.size(); i++)
    {
        solutionBoard.freeBlays->AddBlay(solutionBoard.RemoveBlayIDFromTile(solutionIDs[i]));
    }

    std::cout << solutionBoard.Format() << std::endl;
}