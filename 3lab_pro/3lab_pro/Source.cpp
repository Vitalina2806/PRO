#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;


	int  arr2[4], arr[20] = { 9, 8, 7, 6, 0, 4, 3, 2, 1, 0, 6, 7, 8, 45,33,89,4,23, 12, 17 },bf[8],*buf, t=4;
	char  buf_for_msg[35];
	MPI_Status status;



	void bubblesort(int* a, int n)
	{
		int i, j;
		for (i = n - 1; i >= 0; i--)
		{
			for (j = 0; j < i; j++)
			{
				if (a[j] > a[j + 1])
					swap(a[j], a[j + 1]);
			}
		}
	}
	void p(int rank, int p1, int p2)
	{
		if (rank == p2)
		{
			buf = new int[t + MPI_BSEND_OVERHEAD];
			MPI_Buffer_attach(buf, t + MPI_BSEND_OVERHEAD);

			MPI_Bsend(arr2, 4, MPI_INT, p1, 0, MPI_COMM_WORLD);

			MPI_Buffer_detach(&buf, &t);

			MPI_Recv(arr2, 4, MPI_INT, p1, 0, MPI_COMM_WORLD, &status);

		}
		if (rank == p1)
		{
			MPI_Recv(bf, 4, MPI_INT, p2, 0, MPI_COMM_WORLD, &status);

			for (int i = 4; i < 8; i++)
			{
				bf[i] = arr2[i - 4];
			}
			bubblesort(bf, 8);
			for (int i = 0; i < 4; i++)
			{
				arr2[i] = bf[i];
			}
			buf = new int[t + MPI_BSEND_OVERHEAD];
			MPI_Buffer_attach(buf, t + MPI_BSEND_OVERHEAD);
			MPI_Bsend(bf + 4, 4, MPI_INT, p2, 0, MPI_COMM_WORLD);
			MPI_Buffer_detach(&buf, &t);

		}
	}

int main(int argc, char* argv[])
{
	int rank, size;
	
	status.count = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	//1 çàâäàííÿ
	if (rank == 1)
		strcpy(buf_for_msg, "I'm msg from 1 process");
	MPI_Bcast(&buf_for_msg, 25, MPI_CHAR, 1, MPI_COMM_WORLD);
	if (rank % 2 == 0)
	{
		cout << "\nProcess " << rank << ": received msg - " << buf_for_msg;
	}


	//2 çàâäàííÿ
	if (size == 5)
	{
		if (rank == 0) 
		{
			cout << "\nProcess " << rank << ": Array: ";

			for (int i = 0; i < 20; i++)
			{
				cout << arr[i] << " ";
			}
		}
		MPI_Scatter(arr, 4, MPI_INT, arr2, 4, MPI_INT, 0, MPI_COMM_WORLD);
		for (int i = 0; i < 4; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				p(rank, i, j);
			}
		}
		MPI_Gather(arr2, 4, MPI_INT, arr, 4, MPI_INT, 0, MPI_COMM_WORLD);

		if (rank == 0)
		{
			cout << "\nProcess " << rank << ": Sorted array: ";
			for (int i = 0; i < 20; i++)
			{
				cout << arr[i] << " ";
			}
		}

	}
	else
		if (rank == 0)
			cout << "\nProcess " << rank << ": Not enough process\n";


	MPI_Finalize();
	return 0;
}
