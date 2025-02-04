#include "adjmatrix.h"

AdjMatrix::AdjMatrix(Board* board)
{
    for(int i = 0; i < (board->rows * board->columns); i++)
    {
        if(board->GetTileByID(i)->type == Tile::FILLED) 
        {
            convertMap.insert(std::make_pair(width, board->GetTileByID(i)));
            width++;
        }
    }

    matrix = new int*[width];
    for(int i = 0; i < width; i++)
    {
        matrix[i] = new int[width];
        for(int j = 0; j < width; j++)
        {
            matrix[i][j] = 0;
        }
    }

    //compare every node to every other node
    for(int node = 0; node < width; node++)
    {
        for(int nodeNeighbor = 0; nodeNeighbor < width; nodeNeighbor++)
        {
            //this == that
            if(node == nodeNeighbor) matrix[node][nodeNeighbor] = 0;
            //this -> that (only need to check 1 way cause the other one will also check on its turn)
            else if(!(convertMap[node]->CanSwapBi(convertMap[nodeNeighbor]))) matrix[node][nodeNeighbor] = 1;
        }
    }
}

void AdjMatrix::Print()
{
    std::cout << "\nADJACENCY MATRIX" << std::endl;
    std::cout << std::setw(5) << "X";
    for(int c = 0; c < width; c++)
    {
        std::cout << std::left << std::setw(5) << convertMap[c]->blay->ToStringMedium();
    }
    std::cout << "\n";
    for(int r = 0; r < width; r++)
    {
        std::cout << std::left << std::setw(5) << convertMap[r]->blay->ToStringMedium();
        for(int c = 0; c < width; c++)
        {
            std::cout << std::setw(5) << std::to_string(matrix[r][c]);
        }
        std::cout << std::endl;
    }
}

void PrintVector(std::vector<int> vec)
{
    std::cout << "[";
    for(int i = 0; i < (int)vec.size(); i++)
    {
        std::cout << std::to_string(vec[i]) << ", ";
    }
    std::cout << "]" << std::endl;
}

void AdjMatrix::StartBronKerbosch(int targetSize)
{
    std::vector<int> R;
    std::vector<int> P;
    std::vector<int> X;

    for(int i = 0; i < width; i++)
    {
        P.push_back(i);
    }
    
    BronKerbosch(R, P, X, targetSize);
}

//Where P is the 'entire' graph, X is the vertices removed from P, and R is the maximal clique
void AdjMatrix::BronKerbosch(std::vector<int> R, std::vector<int> P, std::vector<int> X, int targetSize)
{
    //if P and X = 0, report R as a maximal clique
    if(P.size() == 0 && X.size() == 0) 
    {
        results.push_back(R);
        return;
    }
    //if R is as large as the target size, report R as a maximal clique
    if((int)R.size() == targetSize)
    {
        results.push_back(R);
        return;
    }

    //create a pivot set from P U X and choose a random pivot
    std::vector<int> pivotSet;
    std::set_union(P.begin(), P.end(), X.begin(), X.end(), std::inserter(pivotSet, pivotSet.begin()));
    int pivot = pivotSet[rand() % pivotSet.size()];

    //find neighbors of the pivot (swap conflict) and remove the neighbors from P to find difference set
    std::vector<int> pivotNeighbors;
    for(int i = 0; i < width; i++)
    {
        if(matrix[pivot][i] == 1) pivotNeighbors.push_back(i);
    }
    std::vector<int> difference;
    std::set_difference(P.begin(), P.end(), pivotNeighbors.begin(), pivotNeighbors.end(), std::inserter(difference, difference.begin()));
    
    //iterate through difference set
    for(int i = 0; i < (int)difference.size(); i++)
    {
        //select a vertex from the difference set
        std::vector<int> differenceVertex = {difference[i]};

        //create a new set R (output set) as a union (OR) of R and the difference vertex
        std::vector<int> newR;
        std::set_union(R.begin(), R.end(), differenceVertex.begin(), differenceVertex.end(), std::inserter(newR, newR.begin()));

        //find the neighbors (swap conflict) of the difference vertex
        std::vector<int> differenceNeighbors;
        for(int j = 0; j < width; j++)
        {
            if(matrix[difference[i]][j] == 1) differenceNeighbors.push_back(j);
        }

        //create a new set P (whole graph) as an intersection (AND) of P and difference neighbors set
        std::vector<int> newP;
        std::set_intersection(P.begin(), P.end(), differenceNeighbors.begin(), differenceNeighbors.end(), std::inserter(newP, newP.begin()));

        //create a new set X (vertices removed from P) as a union (OR) of X and the difference neighbors set
        std::vector<int> newX;
        std::set_intersection(X.begin(), X.end(), differenceNeighbors.begin(), differenceNeighbors.end(), std::inserter(newX, newX.begin()));

        //recurse on new set
        BronKerbosch(newR, newP, newX, targetSize);
        
        //remove the difference set from P
        std::vector<int> PCopy = P;
        P.clear();
        std::set_difference(PCopy.begin(), PCopy.end(), differenceVertex.begin(), differenceVertex.end(), std::inserter(P, P.begin()));
        
        //add the difference set to X
        std::vector<int> XCopy = X;
        X.clear();
        std::set_union(XCopy.begin(), XCopy.end(), differenceVertex.begin(), differenceVertex.end(), std::inserter(X, X.begin()));
    }
}