#include "blay.h"
#include "blaylist.h"
#include "tile.h"
#include "tileset.h"
#include "board.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char** argv)
{
    std::vector<std::string> solutions;
    std::cout << "Please enter the puzzle string:\n";
    std::string input;
    std::getline(std::cin, input);
    Board tileBoard(input);

    int tid = -1;
    int tid2 = -1;
    int bid = -1;

    tileBoard.Print();
    tileBoard.freeBlays->Print();
    while(1)
    {
        std::cout << "Enter command:\n";
        std::getline(std::cin, input);
        switch(input[0])
        {
            case 'a': //ADD
                std::cout << "Add which blay to where?\n";
                tileBoard.freeBlays->Print();
                std::getline(std::cin, input);
                bid = stoi(input);
                std::getline(std::cin, input);
                tid = stoi(input);
                if(!tileBoard.freeBlays->Contains(bid) || tileBoard.GetTileByID(tid)->type != Tile::EMPTY) break;
                tileBoard.AddBlayToTile(tileBoard.freeBlays->PopBlay(bid), tid);  
                break;
            case 'r': //REMOVE
                std::cout << "Remove blay from which tile?\n";
                std::getline(std::cin, input);
                tid  = stoi(input);
                if(tileBoard.GetTileByID(tid)->type != Tile::FILLED) break;
                tileBoard.freeBlays->AddBlay(tileBoard.RemoveBlayFromTile(tid));
                break;
            case 'n': //NEW
                std::cout << "Enter blay code for new blay:\n";
                std::getline(std::cin, input);
                tileBoard.freeBlays->AddBlay(new Blay(input));
                break;
            case 'd': //DELETE
                std::cout << "Delete which blay in the list?\n";
                std::getline(std::cin, input);
                bid = stoi(input);
                delete(tileBoard.freeBlays->PopBlay(bid));
                break;
            case 'p': //PRINT
                tileBoard.freeBlays->Print();
                break;
            case 'f': //FORMAT
                std::cout << tileBoard.Format() << std::endl;
                break;
            case 's': //SWAP
                std::cout << "Swap which two tiles?\n";
                std::getline(std::cin, input);
                tid = stoi(input);
                std::getline(std::cin, input);
                tid2 = stoi(input);
                tileBoard.Swap(tid, tid2);
                break;
            case 'm': //MATRIX
                std::cout << "Generating adjacency matrix...\n";
                tileBoard.PrintBlays();
                tileBoard.PrintAdjMatrix();
                break;
            case 'k': //KERBOSCH
                std::cout << "Running Bron-Kerbosch on adjacency matrix...\n";
                if(input[1] == 0) 
                {
                    tileBoard.PrintMaximalCliques(99);
                    solutions = tileBoard.FormatMaximalCliques(99);
                }
                else 
                {
                    tileBoard.PrintMaximalCliques(input[1] - '0');
                    solutions = tileBoard.FormatMaximalCliques(input[1] - '0');
                }
                std::cout << "FORMATTED: " << std::endl;
                for(int i = 0; i < (int)solutions.size(); i++)
                {
                    std::cout << solutions[i] << std::endl;
                }
                break;
            case 'c': //COMPLETE (SOLVE)
                solutions = tileBoard.Solve();
                solutions = Board::CleanSolutions(solutions);
                std::cout << "SOLVED " << std::to_string(solutions.size()) << " TIMES." << std::endl;
                for(int i = 0; i < (int)solutions.size(); i++)
                {
                    std::cout << solutions[i] << std::endl;
                }
                break;
            case 'h': //HELP
                std::cout << "a - Add a blay to a tile\n"
                          << "r - Remove a blay from a tile\n"
                          << "n - Create a new blay\n"
                          << "d - Delete a blay\n"
                          << "p - print the list of free blays (kind of redundant? I forgot)\n"
                          << "f - output the format string of the board and freeblays\n"
                          << "s - swap two blays\n"
                          << "m - output the adjacency matrix if you want that for some reason\n"
                          << "k - run the kerbosch algorithm which finds the max clique size in theory for the current board state (you probably need a full board for this one to mean anything)\n"
                          << "c - complete/solve the current board with the given freeblays. It will output the solution\n"
                          << "ENTER - will just print out the info of the board. This is also printed on each command.\n"
                          << "h - help/print commands\n"
                          << "z - specify a depth to create a puzzle up to that size"
                          << "x - same as z but with debug information"
                          << std::endl;
                break;
            case 'l': //ADJACENCY LIST
                std::cout << "Generating Adjacency List...\n";
                tileBoard.PrintSortedAdjList(); 
                break;
            case 'z': //ZWAG (new brute force algorithm)
                std::cout << "Depth of search?" << std::endl;
                std::getline(std::cin, input);
                std::cout << "Buckle up...\n";
                tileBoard.FastGeneratePuzzle(stoi(input));
                break;
            case 'x': //XWAG (new brute force algorithm)
                std::cout << "Depth of search?" << std::endl;
                std::getline(std::cin, input);
                std::cout << "Buckle up...\n";
                tileBoard.FastGeneratePuzzleDebug(stoi(input));
                break;
            case '\n':
            default: break;
        }
        tileBoard.Print();
        tileBoard.freeBlays->Print();
    }

    return 0;
}