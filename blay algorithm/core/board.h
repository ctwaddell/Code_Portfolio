#pragma once

#include "blay.h"
#include "blaylist.h"
#include "tile.h"
#include "tileset.h"
#include "adjmatrix.h"
#include "adjlist.h"
#include "cull.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

class Board
{
    public:
        int rows;
        int columns;
        int size;
        Tile** tiles;
        Blaylist* freeBlays;
        std::string string;

        Board(std::string input);
        Board(int rows, int columns);
        Board(const Board& board);
        ~Board();

        Tile* GetTileByID(int id);
        void SetTile(int tileid, Tile::TileType type);
        void SetRoot(int tileid);
        int Swap(int tid1, int tid2);
        int AddBlayToTile(Blay* blay, int tileid);
        void AAddBlayToTile(Blay* blay, int tileid);
        Blay* RemoveBlayFromTile(int tileid);
        Blay* RemoveBlayIDFromTile(int blayid);
        bool IsOriginalSolution();

        void ParseBoard(char** characterBoard);
        void ParseBoard(std::vector<std::string> blays);
        void ParseNeighbors();
        void Generate();
        void Generate(int seed);
        void ClearBoard();

        void PrintTileIDS();
        void PrintTileTypes();
        void PrintBlayIDS();
        void PrintBlays();
        void PrintRoots();
        void Print();

        void ListNeighbors();
        void ListBlays();

        std::string Format();

        void PrintMaximalCliques(int targetSize);
        std::vector<std::string> FormatMaximalCliques(int targetSize);
        void PrintAdjMatrix();
        void PrintAdjList();
        void PrintSortedAdjList();
        std::vector<std::string> Solve();
        static std::vector<std::string> CleanSolutions(std::vector<std::string> solutions);
        static int SolutionSize(std::vector<std::string> puzzleStrings);
        int BoardSize();
        std::string GeneratePuzzle();
        void FastGeneratePuzzleDebug(int depth);
        std::vector<int> FastGeneratePuzzle(int depth);
};