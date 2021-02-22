#include "mpi.h"
#include <vector>
#include <numeric>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
    // For MPI Debugging
    int i = 1;
    while (0 == i)
        sleep(5);

    MPI_Init(&argc, &argv);

    int myRank{}, worldSize{};
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    vector<int> myVec(1 + myRank + worldSize);
    iota(begin(myVec), end(myVec), 0);

    MPI_Barrier(MPI_COMM_WORLD);
    vector<int> vecSizes(worldSize);
    int mySize = myVec.size();
    MPI_Allgather(&mySize, 1, MPI_INT, vecSizes.data(), 1, MPI_INT, MPI_COMM_WORLD);
    vector<int> displs(worldSize);
    for (int i = 0; i < worldSize; ++i)
    {
        displs[i] = accumulate(vecSizes.begin(), vecSizes.begin() + i, 0);
    }

    vector<int> allData(accumulate(vecSizes.begin(), vecSizes.end(), 0));
    MPI_Allgatherv(myVec.data(), vecSizes[myRank], MPI_INT, allData.data(), vecSizes.data(), displs.data(), MPI_INT, MPI_COMM_WORLD);

    {
        cout << "\nI am CPU " << myRank << ".\n[";
        for (auto j : allData)
        {
            cout << j << ", ";
        }
        cout << "\b\b].\n";
    }

    MPI_Finalize();
    return 0;
}