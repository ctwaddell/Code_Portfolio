#include "cull.h"

Cull::Cull(Board* board)
{
    //find best tiles
    AdjList adjlist = AdjList(board);
    adjlist.Sort();

    //initialize tilelist to descending order
    //(highest connectedness heuristic)
    for(int i = 0; i < (int)adjlist.list.size(); i++)
    {
        tileQueue.push_back(adjlist.list[i]->node);
    }

    //the blays permute onto the tiles.
}

void Cull::StartRecursionDebug(int targetDepth)
{
    searchDepth = targetDepth;

    std::cout << "SEARCHING FOR PUZZLES OF SIZE " << searchDepth << std::endl;

    for(int i = 0; i <= targetDepth; i++)
    {
        solutionIDs.push_back(std::vector<std::vector<int>>());
    }

    int currentDepth = 0;

    int offset = 0;

    //if all permutations with first tile are tested, it no longer needs to be included afterwards
    //this is essentially seeding {1 -> n} permutations
    for(int i = currentDepth; i < (int)tileQueue.size(); i++)
    {
        Tile* startTile = tileQueue[i];
        Blay* startBlay = startTile->RemoveBlay();

        //establish initial conditions
        freeTiles.push_back(startTile);
        freeBlays.push_back(startBlay);

        std::cout << "BEGINNING RECURSION OF DEPTH " << currentDepth + 1 << std::endl;

        //call recursion
        offset++;
        if(RecurseDebug(currentDepth + 1, offset))
        {
            //if it is true, that means its found ONE OF A valid solution
            //Could terminate here if happy with solution or continue permutations
            //return true;
        }
        else
        {
            //if it is false, that means it found a non-unique solution
            //continue permutations...
        }
        startTile->AddBlay(startBlay);

        freeTiles.pop_back();
        freeBlays.pop_back();
    }

    std::cout << "ALGORITHM COMPLETE. SORTING ANSWERS..." << std::endl;

    for(int i = 0; i < (int)solutionIDs.size(); i++)
    {
        for(int j = 0; j < (int)solutionIDs[i].size(); j++)
        {
            std::sort(solutionIDs[i][j].begin(), solutionIDs[i][j].end());
        }
        std::sort(solutionIDs[i].begin(), solutionIDs[i].end(), [](const std::vector<int>& a, const std::vector<int>& b) {return a[0] < b[0];});
    }

    std::cout << "VALID SOLUTIONS ARE AS FOLLOWS: " << std::endl;
    for(int i = 0; i < (int)solutionIDs.size(); i++)
    {
        std::cout << "SIZE " << i << "< ";
        for(int j = 0; j < (int)solutionIDs[i].size(); j++)
        {
            std::cout << "{ ";
            for(int k = 0; k < (int)solutionIDs[i][j].size(); k++)
            {
                std::cout << solutionIDs[i][j][k] << ", ";
            }
            std::cout << "}, ";
        }
        std::cout << ">" << std::endl;
    }
    std::cout << "END OF SOLUTIONS." << std::endl;

    std::sort(largestSolution.begin(), largestSolution.end());

    std::cout << "LARGEST OR FIRST SOLUTION FOUND (SIZE " << largestSolution.size() << "): < ";
    for(int i = 0; i < (int)largestSolution.size(); i++)
    {
        std::cout << largestSolution[i] << ", ";
    }
    std::cout << ">" << std::endl;
}

bool Cull::RecurseDebug(int currentDepth, int offset)
{
    std::vector<int> key = CreateIndexMap(currentDepth);
    std::vector<int> permutation = CreateIndexMap(currentDepth);

    std::cout << "--------------------------------" << std::endl;
    PrintTileQueue();
    PrintFreeBlays();
    PrintFreeTiles();
    std::cout << "BASE: "; PrintIntVector(key);

    while(1)
    {
        int pointOfFailure = 0;
        Unfuck();
        SmartPermute(permutation, pointOfFailure);
        
        std::cout << "PERMUTATION "; PrintIntVector(permutation);

        int remaining = (int)permutation.size();
        for(int i = 0; i < (int)permutation.size(); i++)
        {
            //problem: can/will get stuck in loop
            if(freeTiles[i]->CheckAndAddBlay(freeBlays[permutation[i]]))
            {
                remaining--;
                continue; //if its placed, continue
            }
            else
            {
                std::cout << "FAILED TO PLACE BLAY AT " << i << ". TERMINATING PERMUTATION." << std::endl;
                pointOfFailure = i;
                break; 
                //if it cant be added, next permutation smart permutation
                //we also need to clean up placed blays for that permutation
            }
        }
        if(remaining > 0) continue;

        std::cout << "FOUND PUZZLE. VERIFYING UNIQUENESS." << std::endl;

        if(VerifyPermutation(key, permutation)) break;
        
        std::cout << "VERIFICATION FAILED. CLOSING BRANCH" << std::endl;

        Unfuck();
        return false;
    }

    std::cout << "VERIFICATION SUCCESSFUL. UNIQUE PUZZLE FOUND." << std::endl;

    Unfuck();

    //if it makes it here, it is a unique solution. Above is merely verification

    //if solution is biggest yet, cache it
    if(largestSolution.size() < freeBlays.size())
    {
        std::cout << "CACHING SIZE " << freeBlays.size() << std::endl;
        largestSolution.clear();
        for(int i = 0; i < (int)freeBlays.size(); i++)
        {
            largestSolution.push_back(freeBlays[i]->id);
        }
    }

    //if solution is big enough, cache it
    if(currentDepth >= searchDepth)
    {
        std::vector<int> currentSolutionIDs;
        for(int i = 0; i < (int)freeBlays.size(); i++)
        {
            std::cout << freeBlays[i]->id << std::endl;
            currentSolutionIDs.push_back(freeBlays[i]->id);
        }
        solutionIDs[currentDepth].push_back(currentSolutionIDs);
        std::cout << "GOING TO DEPTH " << currentDepth - 1 << std::endl;
        return true;
    }

    std::cout << "Going deeper..." << std::endl;

    //iterate over sub-permutations 
    for(int i = offset; i < (int)tileQueue.size(); i++)
    {
        std::cout << "index " << i << std::endl;
        //get next tile/piece
        Tile* startTile = tileQueue[i];
        Blay* startBlay = startTile->RemoveBlay(); //pops a null

        //add to testing lists
        freeTiles.push_back(startTile);
        freeBlays.push_back(startBlay);

        std::cout << "--------------------------------" << std::endl;
        std::cout << "BEGINNING RECURSION OF DEPTH " << currentDepth + 1 << std::endl;
        //call recursion
        offset++;
        if(RecurseDebug(currentDepth + 1, offset))
        {
            //if it is true, that means its found ONE OF A valid solution
            //Could terminate here if happy with solution or continue permutations
            //return true;
        }
        else
        {
            //if it is false, that means it found a non-unique solution
            //continue permutations...
        }
        startTile->AddBlay(startBlay);

        freeTiles.pop_back();
        freeBlays.pop_back();
    }

    //once we run out of pieces to test, return I guess
    std::cout << "END OF BRANCH. GOING UP TO " << currentDepth - 1 << std::endl;
    return false;
}

std::vector<int> Cull::StartRecursion(int targetDepth)
{
    searchDepth = targetDepth;

    for(int i = 0; i <= targetDepth; i++)
    {
        solutionIDs.push_back(std::vector<std::vector<int>>());
    }

    int currentDepth = 0;
    int offset = 0;

    //if all permutations with first tile are tested, it no longer needs to be included afterwards
    //this is essentially seeding {1 -> n} permutations
    for(int i = currentDepth; i < (int)tileQueue.size(); i++)
    {
        Tile* startTile = tileQueue[i];
        Blay* startBlay = startTile->RemoveBlay();

        //establish initial conditions
        freeTiles.push_back(startTile);
        freeBlays.push_back(startBlay);

        //call recursion
        offset++;
        if(Recurse(currentDepth + 1, offset))
        {
            startTile->AddBlay(startBlay);

            freeTiles.pop_back();
            freeBlays.pop_back();

            std::sort(largestSolution.begin(), largestSolution.end());

            // std::cout << "LARGEST OR FIRST SOLUTION FOUND (SIZE " << largestSolution.size() << "): < ";
            // for(int i = 0; i < (int)largestSolution.size(); i++)
            // {
            //     std::cout << largestSolution[i] << ", ";
            // }
            // std::cout << ">" << std::endl;
            return largestSolution;
        }

        startTile->AddBlay(startBlay);

        freeTiles.pop_back();
        freeBlays.pop_back();
    }
    std::sort(largestSolution.begin(), largestSolution.end());

    // std::cout << "LARGEST OR FIRST SOLUTION FOUND (SIZE " << largestSolution.size() << "): < ";
    // for(int i = 0; i < (int)largestSolution.size(); i++)
    // {
    //     std::cout << largestSolution[i] << ", ";
    // }
    // std::cout << ">" << std::endl;
    return largestSolution;
}

bool Cull::Recurse(int currentDepth, int offset)
{
    std::vector<int> key = CreateIndexMap(currentDepth);
    std::vector<int> permutation = CreateIndexMap(currentDepth);

    while(1)
    {
        int pointOfFailure = 0;
        Unfuck();
        SmartPermute(permutation, pointOfFailure);

        int remaining = (int)permutation.size();
        for(int i = 0; i < (int)permutation.size(); i++)
        {
            //problem: can/will get stuck in loop
            if(freeTiles[i]->CheckAndAddBlay(freeBlays[permutation[i]]))
            {
                remaining--;
                continue; //if its placed, continue
            }
            else
            {
                pointOfFailure = i;
                break; 
                //if it cant be added, next permutation smart permutation
                //we also need to clean up placed blays for that permutation
            }
        }
        if(remaining > 0) continue;

        if(VerifyPermutation(key, permutation)) break;
        
        //non-unique puzzle-- failure
        Unfuck();
        return false;
    }

    Unfuck();

    //if it makes it here, it is a unique solution. Above is merely verification

    //if solution is biggest yet, cache it
    if(largestSolution.size() < freeBlays.size())
    {
        largestSolution.clear();
        for(int i = 0; i < (int)freeBlays.size(); i++)
        {
            largestSolution.push_back(freeBlays[i]->id);
        }
    }

    //if solution is of the desired size, short circuit
    if(currentDepth >= searchDepth) return true;

    //iterate over sub-permutations 
    for(int i = offset; i < (int)tileQueue.size(); i++)
    {
        //get next tile/piece
        Tile* startTile = tileQueue[i];
        Blay* startBlay = startTile->RemoveBlay();

        //add to testing lists
        freeTiles.push_back(startTile);
        freeBlays.push_back(startBlay);

        //call recursion
        offset++;
        if(Recurse(currentDepth + 1, offset))
        {
            startTile->AddBlay(startBlay);

            freeTiles.pop_back();
            freeBlays.pop_back();
            return true;
        }

        startTile->AddBlay(startBlay);

        freeTiles.pop_back();
        freeBlays.pop_back();
    }
    return false;

}

void Cull::SmartPermute(std::vector<int>& permutation, int pointOfFailure)
{
    //cycle to the next permutation
    //brute force is probably okay for right now, but we'll want to improve this later
    bool hasNext = std::next_permutation(permutation.begin(), permutation.end());
    if (!hasNext)
    {
        // If no more permutations exist, handle as needed
        // Example: Log error, wrap around, or signal termination
        // For now, we'll wrap back to the first permutation
        std::sort(permutation.begin(), permutation.end());
    }
}

void Cull::Unfuck()
{
    for(int i = 0; i < (int)freeTiles.size(); i++)
    {
        if(freeTiles[i]->type == Tile::FILLED) freeTiles[i]->RemoveBlay();
    }
}

bool Cull::VerifyPermutation(std::vector<int> base, std::vector<int> permutation)
{
    for(int i = 0; i < (int)base.size(); i++)
    {
        if(base[i] != permutation[i]) return false;
    }
    return true;
}

std::vector<int> Cull::CreateIndexMap(int depth)
{
    std::vector<int> indexMap;
    for(int i = 0; i < depth; i++)
    {
        indexMap.push_back(i);
    }
    return indexMap;
}

void Cull::PrintTileQueue()
{
    std::cout << "TILE QUEUE: { ";
    for(int i = 0; i < (int)tileQueue.size(); i++)
    {
        std::cout << tileQueue[i]->ToString() << ", ";
    }
    std::cout << "}" << std::endl;
}

void Cull::PrintFreeBlays()
{
    std::cout << "FREE BLAYS: { ";
    for(int i = 0; i < (int)freeBlays.size(); i++)
    {
        std::cout << freeBlays[i]->ToStringMedium() << ", ";
    }
    std::cout << "}" << std::endl;
}

void Cull::PrintFreeTiles()
{
    std:: cout << "FREE TILES: { ";
    for(int i = 0; i < (int)freeTiles.size(); i++)
    {
        std::cout << freeTiles[i]->ToString() << ", ";
    }
    std::cout << "}" << std::endl;
}

void Cull::PrintIntVector(std::vector<int> printVector)
{
    std::cout << "{ ";
    for(int i = 0; i < (int)printVector.size(); i++)
    {
        std::cout << printVector[i] << ", ";
    }
    std::cout << "}" << std::endl;
}