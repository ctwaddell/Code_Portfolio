#include "blaylist.h"

//DEFAULT METHODS
Blaylist::Blaylist()
{

}

Blaylist::Blaylist(std::string blays)
{
    Parse(blays);
}

Blaylist::~Blaylist()
{
    std::cout << "The Blaylist has been destroyed." << std::endl;
}

std::string Blaylist::ToString()
{
    std::stringstream returnString;
    for(auto i = 0u; i < blaylist.size(); i++)
    {
        returnString << blaylist[i]->ToString() << ", ";
    }
    return returnString.str();
}

void Blaylist::Print()
{
    std::cout << ToString() << std::endl;
}

//ACCESSOR METHODS
bool Blaylist::Contains(int blayid)
{
    for(auto i = 0u; i < blaylist.size(); i++)
    {
        if(blaylist[i]->id == blayid) return true;
    }
    return false;
}

int Blaylist::size()
{
    return (int)blaylist.size();
}

//MUTATOR METHODS
Blay* Blaylist::PopBlay(int blayid)
{
    for(auto i = 0u; i < blaylist.size(); i++)
    {
        if(blaylist[i]->id == blayid)
        {
            Blay* returnBlay = blaylist[i];
            blaylist.erase(blaylist.begin() + i);
            return returnBlay;
        }
    }
    return NULL;
}

Blay* Blaylist::PopBlay()
{
    Blay* returnBlay = blaylist[0];
    blaylist.erase(blaylist.begin());
    return returnBlay;
}

Blay* Blaylist::RemoveBlay(int index)
{
    Blay* returnBlay = blaylist[index];
    blaylist.erase(blaylist.begin() + index);
    return returnBlay;

}

void Blaylist::AddBlay(Blay* blay)
{
    blaylist.push_back(blay);
}

void Blaylist::Sort()
{
    std::sort(blaylist.begin(), blaylist.end(), Blay::CompareBlays);
}

void Blaylist::ReverseSort()
{
    std::sort(blaylist.begin(), blaylist.end(), Blay::ReverseCompareBlays);
}

void Blaylist::Parse(std::string blays)
{
    std::istringstream ss(blays);
    std::string token;
    std::vector<std::string> tokens;
    while(std::getline(ss, token, ','))
    {
        AddBlay(new Blay(token));
    }
}