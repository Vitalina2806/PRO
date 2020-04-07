#define _CRT_SECURE_NO_WARNINGS
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>
using namespace std;




void lvl1(MPI_Group world_group, int rank, int size)
{
	if (size >= 8)
	{
		MPI_Group group_1, group_2, group_union;
		int rank1[] = { 0,1,3,4 }, rank2[] = { 2,4,6,7 };
		int rank_1, rank_2, group_rank, size_of_group_1, size_of_group_2, group_union_size;

		MPI_Group_incl(world_group, 4, rank1, &group_1);//create first group
		MPI_Group_rank(group_1, &rank_1);
		MPI_Group_size(group_1, &size_of_group_1);
		
		MPI_Group_incl(world_group, 4, rank2, &group_2);//create second group
		MPI_Group_rank(group_2, &rank_2);
		MPI_Group_size(group_2, &size_of_group_2);
		
		MPI_Group_union(group_1, group_2, &group_union);//union of fist and second groups
		MPI_Group_rank(group_union, &group_rank);
		MPI_Group_size(group_union, &group_union_size);
			
		if (group_union_size == 0)
			cout << "\n1 lvl. || Error of creating union";
		if (group_rank >= 0)
			cout << "\n1 lvl. || Rank  " << rank << "; Union of 1st and 2nd groups. Rank " << group_rank<<". There is/are " <<group_union_size << " processes.";
		
		MPI_Group_free(&group_1);
		MPI_Group_free(&group_2);
		MPI_Group_free(&group_union);
	}
}


void lvl2(MPI_Group world_group, MPI_Comm comm_world, int size, int rank)
{
	if (size >= 12)
	{
		int ranks3[] = { 0,1,2,3,7,8,9,11 }, group_size, group_rank, message;
		MPI_Comm comm_lvl2;
		MPI_Group group_3;

		//create group 
		MPI_Group_incl(world_group, 8, ranks3, &group_3);
		MPI_Comm_create(comm_world, group_3, &comm_lvl2);
		MPI_Group_size(group_3, &group_size);
		MPI_Group_rank(group_3, &group_rank);


		if (group_rank >= 0)
		{
			
			cout << "\n2 lvl. || Rank " << rank << " is " << group_rank << " in the group\n";
			if (group_rank % 2 != 0) cout << "\n2 lvl. || Rank " << rank << " with number in group " << group_rank << " has odd number in group of processes";

			//mesagge is being sent by 4th proces
			if (group_rank != 0) {
				if (group_rank % 4 == 0)
				{
					message = 4;
					cout << "\n2 lvl. || Process  " << rank << " with number in group " << group_rank << " sent a message for other processes. The message is ' " << message << " ' ";

				}
			}
				MPI_Bcast(&message, sizeof(message), MPI_INT, 4, comm_lvl2);
			//message is being recieved by every 3rd process
			MPI_Group_rank(group_3, &group_rank);
			if (group_rank % 3 == 0)
			{
				cout << "\n2 lvl. || Rank " << rank << " with number " << group_rank << " in the group received a message: ' " << message <<" ' ";
			}
		}

		MPI_Group_free(&group_3);
	}
}

int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Comm comm_world;
	MPI_Group world_group;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm_world);
	MPI_Comm_group(comm_world, &world_group);
	lvl1(world_group, rank, size);

	lvl2(world_group, comm_world, size, rank);

	MPI_Group_free(&world_group);
	MPI_Comm_free(&comm_world);
	MPI_Finalize();
	return 0;
}
