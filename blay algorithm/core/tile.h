#pragma once

#include "blay.h"

#include <iostream>

class Tile
{
    public:
        static int tid;
        enum TileType
        {
            VOID = -1, 
            EMPTY = 0, 
            FILLED = 1,
        };

        int id;
        TileType type;
        Tile* neighbors[4];
        Blay* blay;

        //DEFAULT METHODS
        Tile();
        Tile(TileType type);
        Tile(TileType type, Blay* blay);
        ~Tile();

        std::string ToString();

        //ACCESSOR METHODS
        std::string GetTypeName();
        bool CanSwapBi(Tile* otherTile);
        bool CanSwapMono(Tile* otherTile);
        bool CanAddBlay(Blay* blay);

        //MUTATOR METHODS
        int AddBlay(Blay* blay);
        void AAddBlay(Blay* blay);
        bool CheckAndAddBlay(Blay* blay);
        Blay* RemoveBlay();
        int AddNeighbor(int index, Tile* toAdd);
        void SetRoot(bool isRoot);

        //STATIC METHODS
        static void ResetID();
};