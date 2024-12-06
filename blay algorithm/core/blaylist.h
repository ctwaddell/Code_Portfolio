#pragma once

#include "blay.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

class Blaylist
{
    public:
        std::vector<Blay*> blaylist;

        //DEFAULT METHODS
        Blaylist();
        Blaylist(std::string blays);
        ~Blaylist();

        std::string ToString();
        void Print();

        //ACCESSOR METHODS
        bool Contains(int blayid);
        int size();

        //MUTATOR METHODS
        void AddBlay(Blay* blay);
        Blay* PopBlay(int blayid);
        Blay* PopBlay();
        Blay* RemoveBlay(int index);
        void Sort();
        void ReverseSort();
        void Parse(std::string blays);
};