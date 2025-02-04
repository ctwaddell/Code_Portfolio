#include "adjlist.h"

AdjCell::AdjCell(Tile* node)
{
    this->node = node;
}

AdjList::AdjList(Board* board)
{
    for(int i = 0; i < (board->rows * board->columns); i++)
    {
        if(board->GetTileByID(i)->type == Tile::FILLED) 
        {
            list.push_back(new AdjCell(board->GetTileByID(i)));
        }
    }

    for(int current = 0; current < (int)list.size(); current++)
    {
        for(int neighbor = 0; neighbor < (int)list.size(); neighbor++)
        {            //if cant swap, add the edge
            if(!(list[current]->node->CanSwapBi(list[neighbor]->node))) list[current]->edges.push_back(list[neighbor]->node);
        }
    }
}

void AdjList::Print()
{
    std::cout << "\nADJACENCY LIST" << std::endl;
    for(int i = 0; i < (int)list.size(); i++)
    {
        std::cout << std::right << std:: setw(4) << list[i]->node->blay->ToStringMedium() << ": { ";
        for(int j = 0; j < (int)list[i]->edges.size(); j++)
        {
            std::cout << std::right << std::setw(4) << list[i]->edges[j]->blay->ToStringMedium() << ", ";
        }
        std::cout << "}" << std::endl;
    }
     
}

void AdjList::Sort()
{
    std::sort(list.begin(), list.end(), Compare);
}

bool AdjList::Compare(AdjCell* a, AdjCell* b)
{
    return a->edges.size() > b->edges.size(); //descending
}

/*
    FURTHER STEPS:
    Now that the adj list is created, we need to implement a backtracking sort of algorithm.
    Ideally we use some heuristics to cull down computation time.
    -Remove highest degree nodes first -- more edges implies more unswappabilities. High connected = lower chance of non-unique, as any given free tile is less likely to be 'free'
    -Depth search -- go from highest degree node to one of its neighbors with the highest degree. Should keep path in the 'neighborhood' of all mutually unswappable. Kind of expensive tho.
    -Random -- diversity in puzzles, less operations. Maybe less efficient, though?
    -Bron Kerbosch -- Finding maximum cliques could serve as a good 'launching point', but you would have to store the cliques...

    probably either 1 or 3, but we shall see

    Regardless, then you need to actually implement this. Assuming recursion you would need:
    PuzzleGenerator(AdjList, int targetsize, int depth, Board* board, )

    we could probably get away with using 1 board if we are smart.
    If each blay keeps a reference to where it began, it could 'undo' the level of recursion by popping the last blay in the stack back to its origin tile

    It would probably be wise to keep a list of tiles to check sorted one way,
    and a list of blays to check with sorted the other way.
    this would ensure the most recent piece is checked on the oldest spots first, which might
    lead to a little efficiency. We know the most recent piece works on the most recent tile

    if the algorithm finds a valid board, this means everything UP TO that point is also valid.
    we would probably want to store the most recent 'best' board yet. We could keep a vector of valid boards for that and beyond?
    Like a double vector starting at solution size, and then a vector for everything after?

    In theory if it is at depth = targetsize and finds a valid board, we could terminate the algorithm.
    Or we could continue on that branch and terminate if nothing else is found, or we could just keep running the algorithm.

    We need to keep the ordering of the lists consistent, maybe using loops in the recursion. This way, if we
    know a path is locked off, we can avoid it in the future. This poses a slight conundrum, though. Consider:

    Path 0 -> 0, 1, 2 (dies at 2)
    Path 1 -> 0, 1, 3, would it pick 2 back up? we know 0, 1, 2 fails, so 0, 1, 3, 2 inherently fails.
    In this sense, it would be smart to keep track of that somehow.
    We could only check the ones after the most recent index, and build it up that way?
    Like if 0, 1 fails, everything else beyond it would fail. However,
    0, 2, 3, might not. This might be somewhat 'predictable' though. It would always start with removing piece 0,
    which might be a bit patternistic. We could always shuffle the adjacency list before we actually run the algorithm.
    This would help reduce the top left always being chosen first, and more likely to be removed
    (opposite problem with the bottom right).

    assuming a random ordering of the lists, we could still follow the same pattern, though I think.
    8, 2, 3 (dies) invalidates all further combos
    8, 2, 6, 7 (dies)
    8, 2, 6, 1, ... (all die)
    2, 3, 6, ...
    Essentially, if the queue is 'fixed' it should be okay

    random is probably the best for this approach

    Is the adjacency list even necessary? Isn't it useless without a heuristic valuing it?
    We could sort the adjacency list by degree, and then iterate down maybe. This would be 'random' but still fixed
    9, 8, 7, 6 (dies)
    9, 8, 7, 5, 4 (dies)
    9, 8, 7, 5, 3, 2, 1 ding ding ding

    A puzzle can be said to be solved if all the pieces are placed. 
    Assuming the pieces are only placed if its a valid move,
    If all pieces are placed, and any of them are not on their native tile, the puzzle is non-unique

    So essentially it would have something like this:

    TileList freeTiles = [1, 2, 3] where tiles are the removed blays native spots
    BlayList freeBlays = <9, 8, 7> where 9 -> 3, 8 -> 2, etc.

    iterate through free blays and place it front to back (if it can be)
    if free blays 'empties', iterate through them and check if their current tile is their native tile
    if not, invalid branch
    if so, so far so good

    in theory, the way we set up our thing will result in the last permutation being the only correct one, right?
    Like
    [4, 3, 2, 1]
    <A, B, C, D> where D and 4 are most recent

    just imagine:
    [A, 3, 2, 1]
    ...
    [A, B, 2, 1] 
    C cannot be placed on 2
    [A, B, 2, C]
    ...
    [A, B, D, C] 
    non unique board, close branch
    OR
    [A, B, 2, C] 
    D cannot be placed prior permutation
    ...
    [A, 3, B, 1]
    [A, C, B, 1] 
    D cannot be placed prior permutation
    [A, 3, 2, B]
    [A, C, 2, B]
    D cannot be placed prior permutation
    ...

    A piece you cannot place indicates you need to go to the next permutation
    that is because if it cant be placed for that permutation, it wouldnt for any sub permutations
    hence if you say
    [4, 3, 2, 1] A cannot be placed on 4
    all the other checks dont matter 

    because we need to essentially verify all other permutations DONT WORK to 'succeed'
    if we imagine it as a tree, chop of the branch if theres an invalid placement.
    We'll either find a non-unique solution, a unique solution, or a dead end

    -if we find a non unique solution TERMINATE THE ENTIRE RECURSION
    -if we find a dead end TERMINATE PERMUTATION BRANCH
    -continue until unique solution is affirmed



    induction:
    say we know a board up to size k is uniquely solvable
    as we take piece k + 1 from the board, we then need to test all permutations.
    so first we test piece 1 on tile k + 1 (or something like that)
    if piece 1 fits, we then test piece 2 on tile k
    we repeat this, and sooner or later we will either:

    -place all tiles validly, (before last permutation) which implies a non-unique solution. close the branch
    -fail to place a tile, close permutation branch

    if we do the second, that just means we have to try a different permutation.
    then, if we make it through all permutations without ever placing all pieces, 
    that means removing k + 1 pieces results in a valid board. 

    pseudocode:
    Recursion(tileset, pieceset, targetdepth):
    
    cache base permutation (valid solution order?)

    while(1) 
    {
        check permutation
        {
            if (solution)
            {
                if(permutation != cached permutation) faulty recursion, return
                else is the right answer, break;
            }
            if (invalid permutation) continue
        }

        theoretically should never get here. maybe a print statement just in case.
    }

    if target depth, cache answer and return

    remove piece and add stuff to new tile/piece set
    
    Recursion(newTileset, newPieceset, targetdepth)


    also note since the actual 'permutations' arent going to be an array of tiles, itll probably have to be a mapping


    say we have all tiles/pieces sorted by edges
    [5, 2, 4, 1, 3]

    our permutation will be
    [1, 2, 3] where 1, 2, 3 are the indices to the tile list

    theres a little bit of logic with the board and all that, but it should be fine.

    a blay placed on a non-native tile does not imply failure.
    We're not checking if a blay should/shouldnt be at a given spot
    We're checking if all blays can be placed

*/

