#include "blay.h"

int Blay::bid = 0;

//DEFAULT METHODS
Blay::Blay()
{
    id = bid++;
    color = VOID;
    height = 0;
    isRoot = false;
}

Blay::Blay(Color color, int height, bool isRoot)
{
    id = bid++;
    this->color = color;
    this->height = height;
    this->isRoot = isRoot;
}

Blay::Blay(std::string blay)
{
    id = bid++;
    ParseBlay(blay);
    isRoot = false;
}

Blay::~Blay()
{
    //std::cout << "Blay " << this->ToString() << " has been destroyed.\n";
}

std::string Blay::ToString()
{
    return "<ID: " + std::to_string(id) + " (" + GetColorName()[0] + std::to_string(height) + ")>";
}

std::string Blay::ToStringShort()
{
    return GetColorName()[0] + std::to_string(height);
}

std::string Blay::ToStringMedium()
{
    return std::to_string(id) + GetColorName()[0] + std::to_string(height);
}

//ACCESSOR METHODS
std::string Blay::GetColorName()
{
    switch(color)
    {
        case(RED): return "RED";
        case(ORANGE): return "ORANGE";
        case(YELLOW): return "YELLOW";
        case(GREEN): return "GREEN";
        case(BLUE): return "BLUE";
        case(VOID):
        default: return "VOID";
    }
}

//MUTATOR METHODS
int Blay::ParseBlay(std::string blayString)
{
    if(blayString.length() > 2) return 1;
    color = ParseColor(blayString[0]);
    height = blayString[1] - '0';
    return 0;
}

void Blay::SetRoot(bool isRoot)
{
    this->isRoot = isRoot;
}

//STATIC METHODS
void Blay::ResetID()
{
    bid = 0;
}

Blay::Color Blay::ParseColor(char color)
{
    switch(color)
    {
        case('R'): return Blay::RED;
        case('O'): return Blay::ORANGE;
        case('Y'): return Blay::YELLOW;
        case('G'): return Blay::GREEN;
        case('B'): return Blay::BLUE;
        case('V'):
        case('E'):
        case('W'):
        default: return Blay::VOID;

    }
}

bool Blay::CompareBlayIDS(Blay* b1, Blay* b2)
{
    return b1->id > b2->id;
}

bool Blay::CompareBlays(Blay* b1, Blay* b2)
{
    if(b1->height != b2->height) return b1->height > b2->height; //TALLER HEIGHT
    return b1->color < b2->color; //EARLIER COLOR
}

bool Blay::ReverseCompareBlays(Blay* b1, Blay* b2)
{
    if(b1->color != b2->color) return b1->color > b2->color;
    return b1->height > b2->height;
}