#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <omp.h>
using namespace std;



void second_task();


int main()
{
	srand(time(NULL));

	second_task();

	system("pause");
	return 0;
}



void second_task()
{
#pragma omp parallel
	{
#pragma omp sections
		{
#pragma omp section
			{
				vector<int> arr(100);

				generate(arr.begin(), arr.end(), []() { return rand() % 100 - 50; });
				cout << "\nArray: ";
				copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, " ")); 

				swap(*max_element(arr.begin(), arr.end()), *min_element(arr.begin(), arr.end()));

				cout << "\nArray after swapping: ";
				copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, " "));	

				cout << "Max elem = " << *max_element(arr.begin(), arr.end());
				cout << "Min elem = " << *min_element(arr.begin(), arr.end());
				
					}
#pragma omp section
			{
				constexpr int size = 5;
				vector<vector<float>> arr2(size, vector<float>(size));
				float multUnder = 1, multOver = 1;


				for (int i = 0; i < size; i++)
					generate(arr2[i].begin(), arr2[i].end(), []() { return (rand() % 100 - 50) * 0.01; });
#pragma omp critical 
				{
					cout << "\n\nArray2:";
					for (int i = 0; i < size; i++)
					{
						cout << endl;
						copy(arr2[i].begin(), arr2[i].end(), ostream_iterator<float>(cout, " "));
					}
				}
				for (int i = 0; i < size; i++)
					for (int j = 0; j < size; j++)
					{
						if (i > j)
							if (arr2[i][j] < 0)
								multUnder *= arr2[i][j];
					}

				for (int i = 0; i < size; i++)
					for (int j = 0; j < size; j++)
					{
						if (i < j)
							if (arr2[i][j] < 0)
								multOver *= arr2[i][j];
					}
			
				cout << "\nUnder   " << multUnder << "\nOver  " << multOver << endl;
			}
		}
	}
}