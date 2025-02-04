#include "board.h"
#include "tile.h"
#include "blay.h"
#include "tileset.h"
#include "blaylist.h"
#include "adjmatrix.h"

#include <fstream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <ctime>

std::vector<std::vector<int>> intboard;
int rows;
int columns;
int puzzleSize;
int MINSIZE = 5;
int MAXATTEMPTS = 5;

void DFS(int rbase, int cbase)
{
    int r;
    int c;
    const int vectors[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}}; //R U L D
    //check 4 other neighbors
    for(int i = 0; i < 4; i++)
    {
        r = rbase + vectors[i][0];
        c = cbase + vectors[i][1];

        if(r < 0 || r >= rows) continue;
        if(c < 0 || c >= columns) continue;

        if(intboard[r][c] == 2)
        {
            int probability = std::rand() % 100;

            if (probability < 65)
            {
                intboard[r][c] = 1;
                puzzleSize++;
                DFS(r, c);
            }
            else
            {
                intboard[r][c] = 0;
            }
        }
    }
}

int SmartRemoveCount(int puzzleSize)
{
    if(puzzleSize > 10) return 10;
    return (puzzleSize - ((puzzleSize % 2) + 1));
}

std::string GetDate() 
{
    std::time_t t = std::time(NULL);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%B %d, %Y");

    return oss.str();
}

int main(int argc, char** argv)
{
    rows = std::stoi(argv[1]);
    columns = std::stoi(argv[2]);

    std::srand(time(NULL));

    int boundaryCheck;
    do
    {
        //reset/reinitialize
        puzzleSize = 1;
        intboard.clear();
        intboard.resize(rows); 
        for (int r = 0; r < rows; r++) 
        {
            intboard[r].resize(columns, 2);
        }

        //generate random intboard
        std::pair<int, int> center((rows / 2), (columns / 2));
        intboard[center.first][center.second] = 1;
        DFS(center.first, center.second);

        //verify puzzle is r x c
        boundaryCheck = 0;
        for(int r = 0; r < rows; r++)
        {
            if(intboard[r][0] == 1) boundaryCheck |= (1 << 0);
            if(intboard[r][columns - 1] == 1) boundaryCheck |= (1 << 1);
        }
        for(int c = 0; c < columns; c++)
        {
            if(intboard[0][c] == 1) boundaryCheck |= (1 << 2);
            if(intboard[rows - 1][c] == 1) boundaryCheck |= (1 << 3);
        }
    } while (puzzleSize < MINSIZE || boundaryCheck != 15);

    //replace 2's with 0's
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            if(intboard[r][c] == 2) 
            {
                intboard[r][c] = 0;
            }
        }
    }

    //create character board
    char** characterBoard = new char*[rows];
    for(int r = 0; r < rows; r++)
    {
        characterBoard[r] = new char[columns];
    }

    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            characterBoard[r][c] = std::to_string(intboard[r][c])[0];
        }
    }

    //number of blays to remove
    int removeCount = SmartRemoveCount(puzzleSize);
    int attempts = 0;
    int TOLERANCE = 1000;
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

    std::string solution = solutionBoard.Format();
    for(int i = 0; i < (int)solutionIDs.size(); i++)
    {
        solutionBoard.freeBlays->AddBlay(solutionBoard.RemoveBlayIDFromTile(solutionIDs[i]));
    }
    int bounty = (int)solutionIDs.size() * 100;
    std::string board = solutionBoard.Format();

    board.erase(std::remove(board.begin(), board.end(), '\n'), board.end());
    solution.erase(std::remove(solution.begin(), solution.end(), '\n'), solution.end());

    FILE* outputFile = fopen("dailyPuzzle.txt", "w");

    fprintf(outputFile,
        "{\n"
        "\t\"id\": 0,\n"
        "\t\"board\": \"%s\",\n"
        "\t\"solution\": \"%s\",\n"
        "\t\"state\": \"\",\n"
        "\t\"title\": \"Daily Puzzle - %s\",\n"
        "\t\"moves\": 0,\n"
        "\t\"solved\": false,\n"
        "\t\"bounty\": %d\n"
        "}\n", 
        board.c_str(), solution.c_str(), GetDate().c_str(), bounty);
}