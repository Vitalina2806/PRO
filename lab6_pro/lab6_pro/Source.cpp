#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <omp.h>
using namespace std;

vector<vector<int>> create_matrix(vector<vector<int>> matrix)
{
	srand(time(NULL));
	for (int i = 0; i < matrix.size(); i++)
		generate(matrix[i].begin(), matrix[i].end(), []() { return rand() % 20 + 1; });
	return matrix;
}

void task()
{
	constexpr int m = 12, n = 13;
	vector<vector<int>> matrix(m, vector<int>(n));
	vector<int> num_more_than_eleven(m);

	matrix = create_matrix(matrix);

#pragma omp parallel num_threads(m) shared(matrix) 
	{
#pragma omp for
		for (int i = 0; i < m; i++)
		{
			copy_if(matrix[i].begin(), matrix[i].end(), num_more_than_eleven.begin(), [](int a) {return a > 11; });
#pragma omp critical 
			{

				cout << "MAX element in " << i << "th row is " << *std::max_element(num_more_than_eleven.begin(), num_more_than_eleven.end()) << endl;
			}
		}
	}
	cout << endl;
}





int main()
{
	
	task();
		system("pause");
	return 0;
}



