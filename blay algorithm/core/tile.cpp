#include "tile.h"

int Tile::tid = 0;

//DEFAULT METHODS
Tile::Tile()
{
    id = tid++;
    type = VOID;
    for(int i = 0; i < 4; i++)
    {
        neighbors[i] = NULL;
    }
    blay = NULL;
}

Tile::Tile(TileType type)
{
    id = tid++;
    this->type = type;
    for(int i = 0; i < 4; i++)
    {
        neighbors[i] = NULL;
    }
    blay = NULL;
}

Tile::Tile(TileType type, Blay* blay)
{
    id = tid++;
    this->type = type;
    for(int i = 0; i < 4; i++)
    {
        neighbors[i] = NULL;
    }
    AddBlay(blay);
}

Tile::~Tile()
{
    //std::cout << "Tile " << ToString() << "has been destroyed.\n";
}

std::string Tile::ToString()
{
    if(blay != NULL) return "[ID: " + std::to_string(id) + " (" + blay->ToString() + ")]";
    else return "[ID: " + std::to_string(id) + " (NO BLAY)]";
}

//ACCESSOR METHODS
std::string Tile::GetTypeName()
{
    switch(type)
    {
        case(VOID): return "VOID";
        case(EMPTY): return "EMPTY";
        case(FILLED): return "FILLED";
        default: return "VOID";
    }
}

bool Tile::CanSwap(Tile* otherTile)
{
    if(type == VOID || otherTile->type == VOID) return false;
    if(type == EMPTY || otherTile->type == EMPTY) return true;
    if(blay->color == otherTile->blay->color && blay->height == otherTile->blay->height) return false;

    for(int n = 0; n < 4; n++) //This blay with those neighbors
    {
        if(otherTile->neighbors[n] == NULL || otherTile->neighbors[n]->type != FILLED || otherTile->neighbors[n]->id == id) continue;
        if(blay->color == otherTile->neighbors[n]->blay->color) return false;
        if(std::abs(blay->height - otherTile->neighbors[n]->blay->height) >= 2) return false;
    }
    for(int n = 0; n < 4; n++) //That blay with these neighbors
    {
        if(neighbors[n] == NULL || neighbors[n]->type != FILLED || neighbors[n]->id == otherTile->id) continue;
        if(otherTile->blay->color == neighbors[n]->blay->color) return false;
        if(std::abs(otherTile->blay->height - neighbors[n]->blay->height) >= 2) return false;
    }
    return true;
}

//MUTATOR METHODS
int Tile::AddBlay(Blay* blay)
{
    if(type != EMPTY) return 1;
    type = FILLED;
    this->blay = blay;
    return 0;
}

void Tile::AAddBlay(Blay* blay)
{
    type = FILLED;
    this->blay = blay;
}

Blay* Tile::RemoveBlay()
{
    if(type != FILLED) return NULL;
    Blay* returnBlay = blay;
    returnBlay->isRoot = false;
    type = EMPTY;
    blay = NULL;
    return returnBlay;
}

int Tile::AddNeighbor(int index, Tile* toAdd)
{
    if(neighbors[index] != NULL) return 1;
    neighbors[index] = toAdd;
    return 0;
}

void Tile::SetRoot(bool isRoot)
{
    if(blay == NULL || type != FILLED) return;
    blay->SetRoot(isRoot);
}

//STATIC METHODS
void Tile::ResetID()
{
    tid = 0;
}
