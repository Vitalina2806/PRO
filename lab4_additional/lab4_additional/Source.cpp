#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int fib(int n)
{
	if (n <= 1)
		return n;
	return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[])
{
	int size, rank, rank_group1, rank_group2, mesg, buf;
	
	MPI_Group world_group, group1, group2;
	MPI_Comm comm_world, comm_group1, comm_group2, inter_comm;
	MPI_Status status;


	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm_world);//world group
	MPI_Comm_group(comm_world, &world_group);

	
	if (size > 3 && size < 30)
	{	
	int k = (size / 1.6);
	int* ranks1 = new int[k];
	int* ranks2 = new int[size - k];

		for (int i = 0; i < k; i++) {
			ranks1[i] = i;
			if (rank == 0) {
				cout << " " << ranks1[i];
			}
		}

		if (rank == 0) {
			cout << endl;
		}

		int z = 0;
		for (int i = k; i < size; i++) {
			ranks2[z] = i;	
			if (rank == 0) {
				cout << " " << ranks2[z];
			}
			z++;
		}


		MPI_Group_incl(world_group, k, ranks1, &group1);
		MPI_Group_incl(world_group, (size-k), ranks2, &group2);

		//create groups
		
		MPI_Comm_create(comm_world, group1, &comm_group1);
		MPI_Comm_create(comm_world, group2, &comm_group2);
	

		MPI_Group_rank(group1, &rank_group1);
		MPI_Group_rank(group2, &rank_group2);


		if (rank_group1 >= 0) {
			MPI_Intercomm_create(comm_group1, 0, MPI_COMM_WORLD, k, 23, &inter_comm);
			if (rank_group1 == 0) 
			{
				MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 23, inter_comm, &status);
				cout <<"\nReceived fibonacci number=" <<buf<<endl;
			}

		}
		else if(rank_group2 >= 0) {
			MPI_Intercomm_create(comm_group2, 0, MPI_COMM_WORLD, 0, 23, &inter_comm);
			if (rank_group2 == 0)
			{
				mesg = fib(size-k);
				MPI_Send(&mesg, 1, MPI_INT, 0, 23, inter_comm);
			}
		}
			
		MPI_Group_free(&group1);
		MPI_Group_free(&group2);
	}

	MPI_Group_free(&world_group);
	MPI_Comm_free(&comm_world);
	MPI_Finalize();
	return 0;
}