#pragma once

#include "board.h"
#include "tile.h"
#include "blay.h"
#include "adjlist.h"

#include <map>
#include <random>
#include <set>
#include <algorithm>

class Cull
{
    public:
        Board* board;

        std::vector<Tile*> tileQueue; //to choose from, sorted by adjlist
        std::vector<int> indexMap; //maps index to correct tile

        std::vector<Tile*> freeTiles;
        std::vector<Blay*> freeBlays;

        std::vector<std::vector<std::vector<int>>> solutionIDs;
        std::vector<int> largestSolution;

        std::vector<std::vector<int>> results;

        int searchDepth;

        Cull(Board* board);


        //DEFAULT METHODS
        void StartRecursionDebug(int targetDepth);
        std::vector<int> StartRecursion(int targetDepth);
        bool Recurse(int currentDepth, int offset);
        bool RecurseDebug(int currentDepth, int offset);

        void SmartPermute(std::vector<int>& permutation, int pointOfFailure);

        void Unfuck();

        bool VerifyPermutation(std::vector<int> base, std::vector<int> permutation);
        std::vector<int> CreateIndexMap(int depth);

        void PrintTileQueue();
        void PrintFreeBlays();
        void PrintFreeTiles();
        void PrintIntVector(std::vector<int> printVector);
};