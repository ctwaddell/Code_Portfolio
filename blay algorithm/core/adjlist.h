#pragma once

#include "board.h"
#include "tile.h"
#include "blay.h"

#include <map>
#include <random>
#include <set>
#include <algorithm>

class AdjCell
{
    public:
        Tile* node;
        std::vector<Tile*> edges;

        //DEFAULT METHODS
        AdjCell(Tile* node);
};

class AdjList
{
    public:
        std::vector<AdjCell*> list;
        Board* board;

        //DEFAULT METHODS
        AdjList(Board* board);

        void Print();

        void Sort();
        static bool Compare(AdjCell* a, AdjCell* b);
};

