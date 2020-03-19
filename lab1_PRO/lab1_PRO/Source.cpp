#include <time.h>
#include <mpi.h>
#include <iostream>
#include <cstdlib>
using namespace std;

#pragma comment (lib, "msmpi.lib")

int main(int argc, char** argv)
{

    MPI_Init(&argc, &argv);
    srand(time(NULL));
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0)
    {
        const int numOfelements = 60;
        int arr[numOfelements];
        int multiplication = 1;
        int sum = 0;

        cout << "\nProcess " << rank << endl;
        cout<<" Array:\n";
        for (int i = 0; i < numOfelements; i++)
        {
            arr[i] = rand() % 200 - 100;
            cout << arr[i] << " ";
        }

        for (int i = 0; i < numOfelements; i++)
        {
            if (arr[i] >= 0)
            {
                sum += arr[i];
                      }
            else
            {
                multiplication = multiplication * arr[i];
            }
        }
       
        cout << "Sum:" << sum << endl;
        cout << "Result of multiplication:" << multiplication << endl;

    }
    else if (rank == 1)
    {
        int arr[6][8];

        cout << "\nProcess " << rank << endl;
        cout<< "Array:\n";
        for (int i = 0; i < 6; i++)
        {
            cout << "\n";
            for (int j = 0; j < 8; j++)
            {
                arr[i][j] = rand() % 200-100;
                cout << arr[i][j] << " ";
            }
        }
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 8; j++) {
                if (arr[i][j] < 0)
                    arr[i][j] = 0;
            }
        }
        
        cout << endl<<"Array after changing: " << endl;


        for (int i = 0; i < 6; i++)
        {
            cout << "\n";
            for (int j = 0; j < 8; j++)
            {
                cout << arr[i][j] << " ";
            }
        }
    }
    else
    {
        cout << "Process" << rank << "is undefined\n";
    }


    MPI_Finalize();

    //system("pause");
    return 0;
}