#pragma once

#include "blay.h"
#include "blaylist.h"
#include "tile.h"

#include <algorithm>
#include <climits>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Board;

class Tileset
{
    public:
        std::map<Tile*, Blaylist*> tileset;
        Board* board;

        //DEFAULT METHODS
        Tileset(Board* board);
        ~Tileset();

        void Print();

        //ACCESSOR METHODS
        int GetMinimum();
        int GetMinimumTileID();
        Blaylist* GetMinimumBlaylist();
        int size();

        //MUTATOR METHODS
        void ConstructSets();
        void Sort();
        void ReverseSort();
};