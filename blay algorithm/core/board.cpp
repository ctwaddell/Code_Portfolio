#include "board.h"

Board::Board(std::string input)
{
    std::string token;
    std::vector<std::string> tokens;
    std::stringstream ss;
    string = input;

    Tile::ResetID();
    Blay::ResetID();

    if(input.find('|') == std::string::npos) //NO BLAYLIST SPECIFIED
    {
        ss.str(input);

        while(std::getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        rows = stoi(tokens[0]);
        columns = stoi(tokens[1]);

        tiles = new Tile*[rows];
        for(int i = 0; i < rows; i++)
        {
            tiles[i] = new Tile[columns];
        }

        tokens.erase(tokens.begin());
        tokens.erase(tokens.begin());

        ParseBoard(tokens);

        freeBlays = new Blaylist();
    }
    else //BLAYLIST SPECIFIED
    {
        std::string blaysString = input.substr(input.find('|') + 1, input.size() - 1);
        freeBlays = new Blaylist();
        freeBlays->Parse(blaysString);

        std::string boardString = input.substr(0, input.find('|'));
        ss.str(boardString);

        while(std::getline(ss, token, ','))
        {
            tokens.push_back(token);
        }

        rows = stoi(tokens[0]);
        columns = stoi(tokens[1]);

        tiles = new Tile*[rows];
        for(int i = 0; i < rows; i++)
        {
            tiles[i] = new Tile[columns];
        }

        tokens.erase(tokens.begin());
        tokens.erase(tokens.begin());

        ParseBoard(tokens);
    }
    
}

Board::Board(int rows, int columns)
{
    this->rows = rows;
    this->columns = columns;
    string = "";

    Tile::ResetID();
    Tile::ResetID();
    
    tiles = new Tile*[rows];
    for(int i = 0; i < rows; i++)
    {
        tiles[i] = new Tile[columns];
    }

    freeBlays = new Blaylist();
}

Board::~Board()
{
    //std::cout << "The board has been destroyed.\n";
}

Tile* Board::GetTileByID(int id)
{
    return &tiles[id / columns][id % columns];
}

void Board::SetTile(int tileid, Tile::TileType type)
{
    GetTileByID(tileid)->type = type;
}

void Board::SetRoot(int tileid)
{
    GetTileByID(tileid)->SetRoot(true);
}

int Board::Swap(int tid1, int tid2)
{
    Tile* tile1 = GetTileByID(tid1); //3
    Tile* tile2 = GetTileByID(tid2); //2
    if(tile1->type == Tile::FILLED && tile2->type == Tile::FILLED) //tile 1 has a blay, tile 2 has a blay-- swap them
    {
        Blay* blay1 = tile1->RemoveBlay();
        Blay* blay2 = tile2->RemoveBlay();
        tile1->AddBlay(blay2);
        tile2->AddBlay(blay1);
    }
    else if(tile1->type == Tile::FILLED && tile2->type == Tile::EMPTY) //tile 1 has a blay, tile 2 doesn't have a blay-- move blay1 to tile2
    {
        Blay* blay1 = tile1->RemoveBlay();
        tile2->AddBlay(blay1);
    }
    else if(tile1->type == Tile::EMPTY && tile2->type == Tile::FILLED) //tile 1 doesn't have a blay, tile 2 has a blay-- move blay2 to tile1
    {
        Blay* blay2 = tile2->RemoveBlay();
        tile1->AddBlay(blay2);
    }
    else //tile 1 doesn't have a blay, tile 2 doesn't have a blay-- nothing
    {
        return 1;
    }
    return 0;
}

int Board::AddBlayToTile(Blay* blay, int tileid)
{
    if(GetTileByID(tileid)->type == Tile::EMPTY)
    {
        GetTileByID(tileid)->AddBlay(blay);
        return 0;
    }
    else
    {
        return 1;
    }
}

void Board::AAddBlayToTile(Blay* blay, int tileid)
{
    GetTileByID(tileid)->AddBlay(blay);
}

Blay* Board::RemoveBlayFromTile(int tileid)
{
    return GetTileByID(tileid)->RemoveBlay();
}

void Board::ParseBoard(char** characterBoard) //ONLY USED FOR EMPTY BOARD/GENERATION!
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            if(characterBoard[r][c] != '1') tiles[r][c].type = Tile::VOID;
            else tiles[r][c].type = Tile::EMPTY;
        }
    }
}

void Board::ParseBoard(std::vector<std::string> blays)
{
    size = 0;
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            if(blays[0][0] == 'V' || blays[0][0] == 'W')
            {
                tiles[r][c].type = Tile::VOID;
            }
            else if(blays[0][0] == 'E')
            {
                tiles[r][c].type = Tile::EMPTY;
                size++;
            }
            else
            {
                Blay* newBlay = new Blay(blays[0]);
                tiles[r][c].AAddBlay(newBlay);
                size++;
            }
            blays.erase(blays.begin());
        }
    }
    ParseNeighbors();
}

void Board::ParseNeighbors()
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].type == Tile::VOID) continue;
            if(c != columns - 1) //skip right neighbor
            {
                if(tiles[r][c + 1].type != Tile::VOID) tiles[r][c].neighbors[0] = &tiles[r][c + 1];
            }
            if(r != 0) //skip up neighbor
            {
                if(tiles[r - 1][c].type != Tile::VOID) tiles[r][c].neighbors[1] = &tiles[r - 1][c];
            }
            if(c != 0) //skip left neighbor
            {
                if(tiles[r][c - 1].type != Tile::VOID) tiles[r][c].neighbors[2] = &tiles[r][c - 1];
            }
            if(r != rows - 1) //skip down neighbor
            {
                if(tiles[r + 1][c].type != Tile::VOID) tiles[r][c].neighbors[3] = &tiles[r + 1][c];
            }
        }
    }
}

void Board::Generate(int seed)
{
    srand(seed);
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            //Generate valid colors
            std::vector<Blay::Color> validColors = {Blay::RED, Blay::ORANGE, Blay::YELLOW, Blay::GREEN, Blay::BLUE};
            for(int i = 0; i < 4; i++)
            {
                if(tiles[r][c].neighbors[i] == NULL || tiles[r][c].neighbors[i]->type != Tile::FILLED) continue;
                for(auto j = 0u; j < validColors.size();)
                {
                    if(validColors[j] == tiles[r][c].neighbors[i]->blay->color) validColors.erase(validColors.begin() + j);
                    else j++;
                }
            }

            //Generate valid heights
            std::vector<int> validHeights;
            bool hasOne = false;
            bool hasThree = false;
            for(int i = 0; i < 4; i++)
            {
                if(tiles[r][c].neighbors[i] == NULL || tiles[r][c].neighbors[i]->type != Tile::FILLED) continue; //redundant technically
                if(tiles[r][c].neighbors[i]->blay->height == 1) hasOne = true;
                if(tiles[r][c].neighbors[i]->blay->height == 3) hasThree = true;
            }
            if(hasOne && hasThree) validHeights = {2};
            else if(hasOne && !hasThree) validHeights = {1, 2};
            else if(!hasOne && hasThree) validHeights = {2, 3};
            else validHeights = {1, 2, 3};

            tiles[r][c].AddBlay(new Blay(validColors[rand() % validColors.size()], validHeights[rand() % validHeights.size()], false));
        }
    }
}

void Board::Generate()
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            //Generate valid colors
            std::vector<Blay::Color> validColors = {Blay::RED, Blay::ORANGE, Blay::YELLOW, Blay::GREEN, Blay::BLUE};
            for(int i = 0; i < 4; i++)
            {
                if(tiles[r][c].neighbors[i] == NULL || tiles[r][c].neighbors[i]->type != Tile::FILLED) continue;
                for(auto j = 0u; j < validColors.size();)
                {
                    if(validColors[j] == tiles[r][c].neighbors[i]->blay->color) validColors.erase(validColors.begin() + j);
                    else j++;
                }
            }

            //Generate valid heights
            std::vector<int> validHeights;
            bool hasOne = false;
            bool hasThree = false;
            for(int i = 0; i < 4; i++)
            {
                if(tiles[r][c].neighbors[i] == NULL || tiles[r][c].neighbors[i]->type != Tile::FILLED) continue; //redundant technically
                if(tiles[r][c].neighbors[i]->blay->height == 1) hasOne = true;
                if(tiles[r][c].neighbors[i]->blay->height == 3) hasThree = true;
            }
            if(hasOne && hasThree) validHeights = {2};
            else if(hasOne && !hasThree) validHeights = {1, 2};
            else if(!hasOne && hasThree) validHeights = {2, 3};
            else validHeights = {1, 2, 3};

            std::random_device rd;
            std::mt19937 gen(rd());
            int lowerBound = 0;
            int upperBound = 999;
            std::uniform_int_distribution<> distrib(lowerBound, upperBound);
            int randomNumber = distrib(gen);
            tiles[r][c].AddBlay(new Blay(validColors[randomNumber % validColors.size()], validHeights[randomNumber % validHeights.size()], false));
        }
    }
}

void Board::ClearBoard()
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < rows; c++)
        {
            if(tiles[r][c].type == Tile::FILLED) tiles[r][c].RemoveBlay();
        }
    }
}

void Board::PrintTileIDS()
{
    std::cout << "\nTILE IDS\n";
    std::cout << std::left << std::setw(3) << "X";
    for(int c = 0; c < columns; c++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(c);
    }
    std::cout << "\n";

    for(int r = 0; r < rows; r++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(r);
        for(int c = 0; c < columns; c++)
        {
            std::cout << std::left << std::setw(3) << tiles[r][c].id;
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void Board::PrintTileTypes()
{
    std::cout << "\nTILE TYPES\n";
    std::cout << std::left << std::setw(3) << "X";
    for(int c = 0; c < columns; c++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(c);
    }
    std::cout << "\n";

    for(int r = 0; r < rows; r++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(r);
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].type == Tile::VOID)
            {
                std::cout << std::left << std::setw(3) << "V";
            }
            else if(tiles[r][c].type == Tile::EMPTY)
            {
                std::cout << std::left << std::setw(3) << "E";
            }
            else
            {
                std::cout << std::left << std::setw(3) << "F";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void Board::PrintBlayIDS()
{
    std::cout << "\nBLAY IDS\n";
    std::cout << std::left << std::setw(3) << "X";
    for(int c = 0; c < columns; c++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(c);
    }
    std::cout << "\n";

    for(int r = 0; r < rows; r++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(r);
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].type == Tile::VOID)
            {
                std::cout << std::left << std::setw(3) << "";
            }
            else if(tiles[r][c].blay == NULL)
            {
                std::cout << std::left << std::setw(3) << "-";
            }
            else
            {
                std::cout << std::left << std::setw(3) << std::to_string(tiles[r][c].blay->id);
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void Board::PrintBlays()
{
    std::cout << "\nBLAYS\n";
    std::cout << std::left << std::setw(3) << "X";
    for(int c = 0; c < columns; c++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(c);
    }
    std::cout << "\n";

    for(int r = 0; r < rows; r++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(r);
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].type == Tile::VOID)
            {
                std::cout << std::left << std::setw(3) << "";
            }
            else if(tiles[r][c].blay == NULL || tiles[r][c].type == Tile::EMPTY)
            {
                std::cout << std::left << std::setw(3) << "-";
            }
            else
            {
                std::cout << std::left << tiles[r][c].blay->GetColorName()[0] << std::to_string(tiles[r][c].blay->height) << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void Board::PrintRoots()
{
    std::cout << "\nROOTS\n";
    std::cout << std::left << std::setw(3) << "X";
    for(int c = 0; c < columns; c++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(c);
    }
    std::cout << "\n";

    for(int r = 0; r < rows; r++)
    {
        std::cout << std::left << std::setw(3) << std::to_string(r);
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].type == Tile::VOID)
            {
                std::cout << std::left << std::setw(3) << "";
            }
            else if(tiles[r][c].blay == NULL)
            {
                std::cout << std::left << std::setw(3) << "-";
            }
            else if(tiles[r][c].blay->isRoot)
            {
                std::cout << std::left << std::setw(3) << "1";
            }
            else
            {
                std::cout << std::left << std::setw(3) << "0";
            }
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

void Board::Print()
{
    PrintTileIDS();
    PrintTileTypes();
    PrintBlayIDS();
    PrintBlays();
    PrintRoots();
}

void Board::ListBlays()
{
    std::cout << "\nLIST OF BLAYS\n";
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].blay == NULL) continue;
            std::cout << "Tile " << std::setw(2) << tiles[r][c].id << ": " << tiles[r][c].blay->ToString() << '\n';
        }
    }
}

void Board::ListNeighbors()
{
    std::cout << "\nLIST OF NEIGHBORS\n";
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)\
        {
            std::cout << "Tile " << std::setw(3) << std::left << std::to_string(tiles[r][c].id) << "neighbors: ";
            for(int i = 0; i < 4; i++)
            {
                if(tiles[r][c].neighbors[i] == NULL)
                {
                    std::cout << std::setw(2) << "--" << ", ";
                }
                else
                {
                    std::cout << std::setw(2) << std::right << tiles[r][c].neighbors[i]->id << ", ";
                }
            }
            std::cout << '\n';
        }
    }
}

std::string Board::Format()
{
    std::stringstream ss;

    ss << "\n";
    ss << std::to_string(rows) << "," << std::to_string(columns) << ",";
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            if(tiles[r][c].type == Tile::EMPTY)
            {
                ss << "E0,";
            }
            else if(tiles[r][c].blay == NULL || tiles[r][c].type == Tile::VOID)
            {
                ss << "V0,";
            }
            else 
            {
                ss << tiles[r][c].blay->GetColorName()[0] << tiles[r][c].blay->height << ",";
            }
        }
    }

    if(freeBlays->blaylist.size() > 0)
    {
        ss << '|';
        for(int i = 0; i < freeBlays->size(); i++)
        {
            ss << freeBlays->blaylist[i]->GetColorName()[0] << freeBlays->blaylist[i]->height << ",";
        }
    }
    return ss.str();
}

//This solves the board, returning the different solutions as a string vector. Generally we want this to return a vector of size 1 because that means it is unique
std::vector<std::string> Board::Solve()
{
    Tileset tileSets(this);
    std::vector<std::string> solutions;

    while(freeBlays->blaylist.size() != 0)
    {
        tileSets.ConstructSets();

        if(tileSets.GetMinimum() == 0) //PUZZLE IS INVALID
        {
            return std::vector<std::string>(1, "");
        }
        if(tileSets.GetMinimum() == 1) //PUZZLE CASCADES, ADD SINGLE ELEMENT BLAYLIST BLAY TO BOARD
        {
            Blay* toAdd = tileSets.GetMinimumBlaylist()->PopBlay();
            freeBlays->PopBlay(toAdd->id);
            AddBlayToTile(toAdd, tileSets.GetMinimumTileID());
        }
        else //PUZZLE IS NON-CASCADING, CHECK DIFFERENT COMBINATIONS
        {
            for(int i = 0; i < tileSets.GetMinimum(); i++)
            {
                Blay::ResetID();
                Tile::ResetID();
                Board tempBoard(Format());
                Tileset tempTileset(&tempBoard);

                tempTileset.ConstructSets();
                Blay* toAdd = tempTileset.GetMinimumBlaylist()->RemoveBlay(i);
                tempBoard.freeBlays->PopBlay(toAdd->id);
                tempBoard.AddBlayToTile(toAdd, tempTileset.GetMinimumTileID());
            
                std::vector<std::string> newSolutions = tempBoard.Solve();
                solutions.insert(solutions.end(), newSolutions.begin(), newSolutions.end());
            }
            return solutions;
        }

    }
    solutions.push_back(Format());
    return solutions;
}

//Returns the solutions vector just formatted nicer I think
std::vector<std::string> Board::CleanSolutions(std::vector<std::string> solutions)
{
    solutions.erase(std::remove(solutions.begin(), solutions.end(), ""), solutions.end());
    std::sort(solutions.begin(), solutions.end());
    auto last = std::unique(solutions.begin(), solutions.end());
    solutions.erase(last, solutions.end());
    return solutions;
}

void Board::PrintAdjMatrix()
{
    AdjMatrix adjmatrix(this);
    adjmatrix.Print();
}

void Board::PrintMaximalCliques(int targetSize)
{
    AdjMatrix adjmatrix(this);
    adjmatrix.StartBronKerbosch(targetSize);

    int maxSolSize = 0;
    for(int i = 0; i < (int)adjmatrix.results.size(); i++)
    {
        if((int)adjmatrix.results[i].size() > maxSolSize) maxSolSize = (int)adjmatrix.results[i].size();
    }

    //.std::cout << std::to_string(adjmatrix.results.size()) << " MAX CLIQUES, SIZE " << std::to_string(maxSolSize) << std::endl;
    for(int i = 0; i < (int)adjmatrix.results.size(); i++)
    {
        if((int)adjmatrix.results[i].size() != maxSolSize) continue;
        for(int j = 0; j < (int)adjmatrix.results[i].size(); j++)
        {
            std::cout << adjmatrix.convertMap[adjmatrix.results[i][j]]->ToString() << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "DONE" << std::endl;
}

//returns number of tiles in the puzzle strings blaylist (they are the same size)
int Board::SolutionSize(std::vector<std::string> puzzleStrings)
{
    int size = (int)puzzleStrings[0].size();
    for(int i = 0; i < (int)puzzleStrings.size(); i++)
    {
        if((int)puzzleStrings[i].size() != size) return -1;
    }

    int index;
    for(int i = 0; i < size; i++)
    {
        if(puzzleStrings[0][i] == '|') 
        {
            index = ++i;
            break;
        }
    }
    return ((size - index) / 3);
}

int Board::BoardSize()
{
    return size;
}

std::vector<std::string> Board::FormatMaximalCliques(int targetSize)
{
    std::vector<std::string> returnVector;
    AdjMatrix adjmatrix(this);
    adjmatrix.StartBronKerbosch(targetSize);
    /*
        Note that this doesn't guarantee a unique solution.

        Consider a puzzle where two pieces have an edge. That means they can't be swapped.
        Now, consider three. It is the same.
        However, when you get to four pieces, trouble can arise by the order you remove
        the pieces. When the adjacency matrix is GENERATED, the largest clique is correct.
        However, say you have pieces X, Y, and Z. Say X only shares an edge with Y because Z is there.
        Once you remove Z, X and Y technically can be swapped. I think technically this can only
        occur for even size puzzles, but the principle is, removing neighbor's which guarantee
        edges for other pieces also being removed causes the non-uniqueness to arise. Largely,
        this isn't a major issue though and could probably be solved if needed.
    */

    int maxSolSize = 0;
    for(int i = 0; i < (int)adjmatrix.results.size(); i++)
    {
        if((int)adjmatrix.results[i].size() > maxSolSize) maxSolSize = (int)adjmatrix.results[i].size();
    }

    for(int i = 0; i < (int)adjmatrix.results.size(); i++)
    {
        if((int)adjmatrix.results[i].size() != maxSolSize) continue;
        Board newBoard(string);
        for(int j = 0; j < (int)adjmatrix.results[i].size(); j++)
        {
            newBoard.freeBlays->AddBlay(newBoard.RemoveBlayFromTile(adjmatrix.convertMap[adjmatrix.results[i][j]]->id));
        }
        returnVector.push_back(newBoard.Format());
    }

    return returnVector;
}
