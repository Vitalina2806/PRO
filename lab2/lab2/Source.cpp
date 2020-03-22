

#include <mpi.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <string.h>
#include <vector>
using namespace std;
#pragma comment (lib, "msmpi.lib")


void sendMessage_sync(int size, int rank) {
	int number;
	cout << "\nProcess " << rank << ".Please, enter number ";
	cin >> number;
	cin.get();
	for (int i = 1; i < size; i++) {
		MPI_Ssend(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

}


void Message_sync(int& msg, MPI_Status& status, int rank)
{
	MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

	cout << "\nProcess  " << rank << ".Message was received: " << (rank % 2 == 0 ? msg : 0) << ". '0' means, that message was received by process, but not printed." << endl;

}


int main(int argc, char** argv)
{
	char Y = 'Y', E = 'E';
	int numOfSymbol = (int)(Y);
	char tmp = (char)(numOfSymbol);


	int rank, size, TAG = 0, msg, bufsize = 12;
	char* string_buf = NULL;
	MPI_Status status;

	srand(time(NULL));
	status.count = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	if (size >= 4) {
		if (rank == 0)
		{
			//decrement Y to E
			cout << "\nProcess" << rank << ". Symbol  " << tmp;
			MPI_Send(&numOfSymbol, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			do
			{
				MPI_Recv(&numOfSymbol, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
				tmp = (char)(numOfSymbol);
				cout << "\nProcess " << rank << ". Symbol after decrementing  " << tmp;
				numOfSymbol--;
				MPI_Send(&numOfSymbol, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			} while (numOfSymbol > (int)(E));

			//send message
			string_buf = (char*)malloc(bufsize + MPI_BSEND_OVERHEAD);
			MPI_Buffer_attach(string_buf, bufsize + MPI_BSEND_OVERHEAD);
			string_buf = (char*)"Take care!";
			MPI_Bsend(&string_buf, bufsize, MPI_CHAR, 1, TAG, MPI_COMM_WORLD);
			MPI_Buffer_detach(&string_buf, &bufsize);
			MPI_Recv(&string_buf, bufsize, MPI_CHAR, 3, TAG, MPI_COMM_WORLD, &status);

			//send number from keyboard
			sendMessage_sync(size, rank);

		}
		else if (rank == 1)
		{
			for (int i = numOfSymbol; i >= (int)(E); i--)
			{
				MPI_Recv(&numOfSymbol, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
				tmp = (char)(numOfSymbol);
				cout << "\nProcess " << rank << ". Symbol after decrementing  " << tmp;
				MPI_Send(&numOfSymbol, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

			}

			string_buf = (char*)malloc(bufsize + MPI_BSEND_OVERHEAD);
			MPI_Buffer_attach(string_buf, bufsize + MPI_BSEND_OVERHEAD);
			MPI_Recv(&string_buf, bufsize, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &status);
			cout << "\nProcess " << rank << ". Message is: " << string_buf << endl;
			MPI_Bsend(&string_buf, bufsize, MPI_CHAR, 2, TAG, MPI_COMM_WORLD);
			MPI_Buffer_detach(&string_buf, &bufsize);



			Message_sync(msg, status, rank);
		}
		else if (rank == 2)
		{

			string_buf = (char*)malloc(bufsize + MPI_BSEND_OVERHEAD);
			MPI_Buffer_attach(string_buf, bufsize + MPI_BSEND_OVERHEAD);
			MPI_Recv(&string_buf, bufsize, MPI_CHAR, 1, TAG, MPI_COMM_WORLD, &status);
			cout << "\nProcess " << rank << ". Message is: " << string_buf << endl;
			MPI_Bsend(&string_buf, bufsize, MPI_CHAR, 3, TAG, MPI_COMM_WORLD);
			MPI_Buffer_detach(&string_buf, &bufsize);

			Message_sync(msg, status, rank);

		}
		else if (rank == 3)

		{
			string_buf = (char*)malloc(bufsize + MPI_BSEND_OVERHEAD);
			MPI_Buffer_attach(string_buf, bufsize + MPI_BSEND_OVERHEAD);
			MPI_Recv(&string_buf, bufsize, MPI_CHAR, 2, TAG, MPI_COMM_WORLD, &status);
			cout << "\nProcess " << rank << ". Message is: " << string_buf << endl;
			MPI_Bsend(&string_buf, bufsize, MPI_CHAR, 0, TAG, MPI_COMM_WORLD);
			MPI_Buffer_detach(&string_buf, &bufsize);


			Message_sync(msg, status, rank);

		}
		else
		{
			Message_sync(msg, status, rank);
		}
	}
	else
	{
		cout << "Program is required minimum 4 processes. Please, restart the program." << endl;
	}

	MPI_Finalize();
	return 0;
}