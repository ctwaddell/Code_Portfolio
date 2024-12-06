#include "board.h"
#include "tile.h"
#include "blay.h"
#include "tileset.h"
#include "blaylist.h"
#include "adjmatrix.h"

#include <fstream>
#include <chrono>
#include <stdlib.h>
#include <time.h>

std::vector<std::vector<int>> intboard;
int rows;
int columns;
int puzzleSize;
const int MINSIZE = 5;
const int TOLERANCE = 99;

void DFS(int rbase, int cbase)
{
    int r;
    int c;
    //check 4 other neighbors
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
            case(0): //RIGHT neighbor
                if((cbase + 1) >= columns) continue; // out of bounds
                r = rbase;
                c = cbase + 1;
                break;
            case(1): //UP neighbor
                if((rbase - 1) < 0) continue;
                r = rbase - 1;
                c = cbase;
                break;
            case(2): //LEFT neighbor
                if((cbase - 1) < 0) continue;
                r = rbase;
                c = cbase - 1;
                break;
            case(3): //DOWN neighbor
                if((rbase + 1) >= rows) continue;
                r = rbase + 1;
                c = cbase;
                break;
            default:
                break;
        }

        switch(intboard[r][c])
        {
            case(2): //uninitialized
                if((intboard[r][c] = rand() % 2) == 1) //coin flip creates a tile
                {
                    puzzleSize++;
                    DFS(r, c);
                }
            case(1): //filled
            case(0): //empty
            default:
                break;
        }
    }
}

// ./automatic rows columns
int main(int argc, char** argv)
{
    srand(time(NULL));

    retry:
    intboard.clear();
    puzzleSize = 1;

    if(argc > 1)
    {
        rows = std::stoi(argv[1]);
        columns = std::stoi(argv[2]);
    }
    else
    {
        rows = rand() % 4 + 2; // [2, 5]
        columns = rand() % 4 + 2;
    }

    intboard.resize(rows);

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            intboard[r].push_back(2);
        }
    }

    std::pair<int, int> center(rand() % rows, rand() % columns);

    intboard[center.first][center.second] = 1;

    DFS(center.first, center.second);
    if(puzzleSize < MINSIZE) goto retry;


    std::vector<std::vector<int>> newboard;

    //remove redundant rows
    bool toAdd = false;
    int newrows = 0;
    for(int r = 0; r < rows; r++)
    {
        toAdd = false;
        for(int c = 0; c < columns; c++)
        {
            if(intboard[r][c] == 1) 
            {
                toAdd = true;
                break;
            }
        }
        if(toAdd)
        {
            newrows++;
            newboard.push_back(std::vector<int>{});
            for(int c = 0; c < columns; c++)
            {
                newboard[newrows - 1].push_back(intboard[r][c]);
            }
        }
    }

    //remove redundant columns
    bool toRemove = true;
    int newcolumns = columns;
    for(int c = (columns - 1); c >= 0; c--)
    {
        toRemove = true;
        for(int r = 0; r < newrows; r++)
        {
            if(newboard[r][c] == 1) toRemove = false;
        }
        if(toRemove)
        {
            newcolumns--;
            for(int r = 0; r < newrows; r++)
            {
                newboard[r].erase(newboard[r].begin() + c);
            }
        }
    }

    //replace 2's with 0's
    for(int r = 0; r < newrows; r++)
    {
        for(int c = 0; c < newcolumns; c++)
        {
            if(newboard[r][c] == 2) 
            {
                newboard[r][c] = 0;
            }
        }
    }

    // //format string
    // std::cout << newrows << "," << newcolumns << ",";
    // for(int r = 0; r < newrows; r++)
    // {
    //     for(int c = 0; c < newcolumns; c++)
    //     {
    //         std::cout << newboard[r][c];
    //     }
    // }

    //create character board
    char** characterBoard = new char*[newrows];
    for(int r = 0; r < newrows; r++)
    {
        characterBoard[r] = new char[newcolumns];
    }

    for(int r = 0; r < newrows; r++)
    {
        for(int c = 0; c < newcolumns; c++)
        {
            characterBoard[r][c] = std::to_string(newboard[r][c])[0];
        }
    }

    //remove count math
    int removecount;
    switch(puzzleSize)
    {
        case(5):
            removecount = 3;
            break;
        case(6):
            removecount = 4;
            break;
        case(7):
            removecount = 4;
            break;
        case(8):
            removecount = 5;
            break;
        case(9):
            removecount = 6;
            break;
        case(10):
            removecount = 6;
            break;
        case(11):
            removecount = 7;
            break;
        case(12):
            removecount = 8;
            break;
        case(13):
            removecount = 8;
            break;
        case(14):
            removecount = 9;
            break;
        case(15):
            removecount = 9;
            break;
        case(16):
            removecount = 10;
            break;
        case(17):
            removecount = 10;
            break;
        case(18):
            removecount = 10;
            break;
        case(19):
            removecount = 11;
            break;
        case(20):
            removecount = 11;
            break;
        case(21):
            removecount = 11;
            break;
        case(22):
            removecount = 12;
            break;
        case(23):
            removecount = 12;
            break;
        case(24):
            removecount = 12;
            break;
        case(25):
            removecount = 12;
            break;
        default:
            removecount = 12;
            break;
    }
    
    int pity = 0;
    while(1)
    {
        std::vector<std::string> puzzleStrings;

        do 
        {
            Board tileBoard(newrows, newcolumns);
            tileBoard.ParseBoard(characterBoard);
            tileBoard.ParseNeighbors();
            tileBoard.Generate();
            tileBoard.string = tileBoard.Format();
        
            puzzleStrings = tileBoard.FormatMaximalCliques(removecount);
            if (Board::SolutionSize(puzzleStrings) < removecount) 
            {
                pity++;
            }
            else
            {
                break;
            }

            if (pity > TOLERANCE)
            {
                std::cout << "Pity level broken" << std::endl;
                break;
            }
        } while (true);

        for(int i = 0; i < (int)puzzleStrings.size(); i++)
        {
            Board solveBoard(puzzleStrings[i]);
            std::vector<std::string> solvability = solveBoard.Solve();
            solvability = Board::CleanSolutions(solvability);
            if(solvability.size() == 1) //size 1 means it is a unique solution
            {
                int bounty = (Board::SolutionSize(puzzleStrings) / 2) * 100;

                std::ofstream file("automaticpuzzle.txt");
                puzzleStrings[i].erase(0, 1);
                solvability[0].erase(0, 1); //get rid of newlines...
                file << "{" << std::endl;
                file << "\t\"id\": 0," << std::endl;
                file << "\t\"board\": \"" << puzzleStrings[i] << "\"," << std::endl;
                file << "\t\"solution\": \"" << solvability[0] << "\"," << std::endl;
                file << "\t\"state\": \"\"," << std::endl;
                file << "\t\"title\": \"DailyPuzzle\"," << std::endl;
                file << "\t\"moves\": 0," << std::endl;
                file << "\t\"solved\": false," << std::endl;
                file << "\t\"bounty\": " << bounty << std::endl;
                file << "}" << std::endl;
                goto done;
            }        
        }  
    }
    done:
    rows++;

}

