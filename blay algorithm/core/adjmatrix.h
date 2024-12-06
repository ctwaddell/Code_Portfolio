#pragma once

#include "board.h"
#include "tile.h"
#include "blay.h"

#include <map>
#include <random>
#include <set>

class AdjMatrix
{
    public:
        std::map<int, Tile*> convertMap;
        int width = 0; //WIDTH ALWAYS EQUALS HEIGHT-- SQUARE MATRIX
        Board* board;
        int** matrix;
        std::vector<std::vector<int>> results;

        //DEFAULT METHODS
        AdjMatrix(Board* board);

        void Print();

        void StartBronKerbosch(int targetSize);
        void BronKerbosch(std::vector<int> R, std::vector<int> P, std::vector<int> X, int targetSize);
};