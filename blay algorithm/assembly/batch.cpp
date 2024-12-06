#include "board.h"
#include "tile.h"
#include "blay.h"
#include "tileset.h"
#include "blaylist.h"
#include "adjmatrix.h"

#include <fstream>
#include <chrono>

int main(int argc, char** argv)
{
    std::string input;
    int columns;
    int rows;
    int tolerance = 999;
    if(argc > 1) tolerance = std::stoi(argv[1]);

    while(1)
    {
        std::cout << "How wide of the board?" << std::endl;;
        std::getline(std::cin, input);
        if((columns = stoi(input)) == 0 || columns < 0) continue;
        break;
    }

    while(1)
    {
        std::cout << "How tall of the board?" << std::endl;
        std::getline(std::cin, input);
        if((rows = stoi(input)) == 0 || rows < 0) continue;
        break;
    }

    std::cout << "Enter your board:" << std::endl;

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

    std::cout << "Enter blays to remove:" << std::endl;
    std::getline(std::cin, input);

    int toRemove = input[0] == '0' ? INT_MAX : stoi(input);
    if(toRemove > (rows * columns - 1)) toRemove = (rows * columns - 1);


    int solveCount = 0;
    int pity = 0;
    int puzzlesCount = 0;
    //std::vector<std::string> cache;
    std::ofstream file("output.txt");
    std::cout.rdbuf(file.rdbuf());
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    while(solveCount == 0)
    {
        file.close();
        file.open("output.txt");
        Board tileBoard(rows, columns);
        tileBoard.ParseBoard(characterBoard);
        tileBoard.ParseNeighbors();
        tileBoard.Generate();
        tileBoard.string = tileBoard.Format();
        std::vector<std::string> puzzleStrings = tileBoard.FormatMaximalCliques(toRemove);
        if(Board::SolutionSize(puzzleStrings) < toRemove && pity < tolerance) //if the solution size is not equal to what we want, we try again. if we try again enough and fail, we eventually pass through
        {
            pity++;
            continue;
        }

        puzzlesCount = (int)puzzleStrings.size();
        std::cout << "PUZZLE " << std::to_string(columns) << "X" << std::to_string(rows) << " SOLUTIONS, REMOVING (EXPECTED: " << std::to_string(toRemove) << ", ACTUAL: " << std::to_string(Board::SolutionSize(puzzleStrings)) << ") TRY " << std::to_string(pity) << ": " << std::endl;
        tileBoard.PrintBlays();
        for(int i = 0; i < (int)puzzleStrings.size(); i++)
        {
            Board solveBoard(puzzleStrings[i]);
            std::vector<std::string> solvability = solveBoard.Solve();
            solvability = Board::CleanSolutions(solvability);
            if(solvability.size() == 1) //size 1 means it is unique
            {
                std::cout << solvability[0];
                std::cout << puzzleStrings[i] << std::endl;
                solveCount++;
            }        
        }  
    }

    auto end = std::chrono::system_clock::now();
    std::time_t end_c = std::chrono::system_clock::to_time_t(end);
    std::tm* end_tm = std::localtime(&end_c);
    std::cout << "# Puzzles Generated: " << std::to_string(puzzlesCount) << std::endl;
    std::cout << "Unique Solutions: " << std::to_string(solveCount) << std::endl;
    std::cout << "Start time: " << std::put_time(&now_tm, "%T") << std::endl;
    std::cout << "End time: " << std::put_time(end_tm, "%T") << std::endl;
    std::cout << "------------------------------------" << std::endl;
    file.close();

    std::ifstream inputF("output.txt");
    std::ofstream outputF("finalpuzzles.txt", std::ios_base::app);

    std::string line;

    while(std::getline(inputF, line))
    {
        outputF << line << std::endl;
    }

    inputF.close();
    outputF.close();
}