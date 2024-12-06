#pragma once

#include <iostream>
#include <string>
#include <vector>

class Blay
{
    public:
        static int bid;
        enum Color
        {
            VOID = -1,
            RED = 1,
            ORANGE = 2,
            YELLOW = 3,
            GREEN = 4,
            BLUE = 5,
        };
        
        int id;
        Color color;
        int height;
        bool isRoot;

        //DEFAULT METHODS
        Blay();
        Blay(Color color, int height, bool isRoot);
        Blay(std::string blay);
        ~Blay();

        std::string ToString();
        std::string ToStringShort();
        std::string ToStringMedium();

        //ACCESSOR METHODS
        std::string GetColorName();

        //MUTATOR METHODS
        int ParseBlay(std::string blay);
        void SetRoot(bool isRoot);
        
        //STATIC METHODS
        static void ResetID();
        static Color ParseColor(char color);
        static bool CompareBlayIDS(Blay* b1, Blay* b2);
        static bool CompareBlays(Blay* b1, Blay* b2);
        static bool ReverseCompareBlays(Blay* b1, Blay* b2);
};