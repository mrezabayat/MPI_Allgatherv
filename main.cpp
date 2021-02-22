#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int myRank{}, worldSize{};
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    int myArray[myRank+worldSize];
    for (int i = 0; i < myRank+worldSize; ++i)
    {
        myArray[i] = i + myRank;
    }

    int recvBuff[5]; // Let's assume we are running with 2 processes
    int recvCounts[] = {2,3};
    int disp[] = {0, 2};
    MPI_Allgatherv(myArray, myRank + worldSize, MPI_INT, recvBuff, recvCounts, disp, MPI_INT, MPI_COMM_WORLD);

    {
        cout << "\nI am CPU " << myRank << ".\n[";
        for (int j = 0; j<5; j++)
        {
            cout << j << ", ";
        }
        cout << "\b\b].\n";
    }
    
    MPI_Finalize();
    return 0;
}