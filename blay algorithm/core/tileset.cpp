#include "board.h"
#include "tileset.h"

//DEFAULT METHODS
Tileset::Tileset(Board* board)
{
    this->board = board;
}

Tileset::~Tileset()
{
    //std::cout << "The Tileset has been destroyed" << std::endl;
}

void Tileset::Print()
{
    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
        std::cout << "Tile " << std::setw(2) << std::to_string(iterator->first->id) << ": {";
        for(auto j = 0u; j < iterator->second->blaylist.size(); j++)
        {
            std::cout << iterator->second->blaylist[j]->ToString() << ", ";
        }
        std::cout << "}\n";
    }
    std::cout << std::endl;
}

void Tileset::Sort()
{
    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
       iterator->second->Sort();
    }
}

void Tileset::ReverseSort()
{
    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
       iterator->second->Sort();
    }
}

//ACCESSOR METHODS
Blaylist* Tileset::GetMinimumBlaylist()
{
    int minimum = GetMinimum();
    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
        if(iterator->second->size() == minimum) return iterator->second;
    }
    return {};
}

int Tileset::GetMinimumTileID()
{
    int minimum = GetMinimum();
    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
        if((int)iterator->second->size() == minimum) return iterator->first->id;
    }
    return -1;
}

int Tileset::GetMinimum()
{
    int minimum = INT_MAX;
    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
        if(iterator->second->size() < minimum) minimum = iterator->second->size();
    }
    return minimum;
}

int Tileset::size()
{
    return tileset.size();
}

//MUTATOR METHODS
void Tileset::ConstructSets()
{
    tileset.clear();

    for(int r = 0; r < board->rows; r++)
    {
        for(int c = 0; c < board->columns; c++)
        {
            if(board->tiles[r][c].type == Tile::FILLED) board->tiles[r][c].SetRoot(true);
            if(board->tiles[r][c].type == Tile::EMPTY) tileset.insert(std::make_pair(&board->tiles[r][c], new Blaylist()));
        }
    }

    for(auto iterator = tileset.begin(); iterator != tileset.end(); iterator++)
    {
        Tile* currentTile = iterator->first;
        for(int b = 0; b < board->freeBlays->size(); b++)
        {
            bool canAdd = true;
            for(int n = 0; n < 4; n++)
            {
                if(currentTile->neighbors[n] == NULL || currentTile->neighbors[n]->type != Tile::FILLED) continue; //IF TILE IS OF NO CONSEQUENCE, SKIP
                if(board->freeBlays->blaylist[b]->color == currentTile->neighbors[n]->blay->color) canAdd = false; //IF COLOR CONFLICTS, CAN'T ADD
                if(std::abs(board->freeBlays->blaylist[b]->height - currentTile->neighbors[n]->blay->height) >= 2) canAdd = false; //IF HEIGHT CONFLICTS, CAN'T ADD    
            }

            if(canAdd) iterator->second->AddBlay(board->freeBlays->blaylist[b]);
        }
    }
}



