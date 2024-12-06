#include "blay.h"
#include "blaylist.h"
#include "tile.h"
#include "tileset.h"
#include "board.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char** argv)
{
    std::stringstream blaysInput;
    std::string input;

    std::cout << "Please enter the puzzle string:\n";
    std::getline(std::cin, input);

    Board tileBoard(input);

    if(input.find('|') != std::string::npos) goto SOLVE;

  
    std::cout << "Please enter the number of blays to remove:\n";
    std::getline(std::cin, input);;

    return 0;

    SOLVE:
    std::vector<std::string> solutions = tileBoard.Solve();
    solutions.erase(std::remove(solutions.begin(), solutions.end(), ""), solutions.end());
    std::sort(solutions.begin(), solutions.end());
    auto last = std::unique(solutions.begin(), solutions.end());
    solutions.erase(last, solutions.end());
    std::cout << "SOLVED " << std::to_string((int)solutions.size()) << " TIMES:";
    for(const auto& element : solutions)
    {
        std::cout << element;
    }
    std::cout << std::endl;
    
    return 0;
}