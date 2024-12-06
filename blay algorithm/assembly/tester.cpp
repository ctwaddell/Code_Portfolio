#include "blay.h"
#include "blaylist.h"
#include "tile.h"
#include "tileset.h"
#include "board.h"

#include <iostream>
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
            case '\n':
            default: break;
        }
        tileBoard.Print();
        tileBoard.freeBlays->Print();
    }

    return 0;
}