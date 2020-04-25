#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <omp.h>
#include <array>
#include <time.h>
#include <vector>
#include <chrono>


using namespace std;


class Timer
{
public:
	Timer() {}
	~Timer() {}
	void start_Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	double stop_Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
};

void first_task()
{
	constexpr int m = 13, n = 13;
	vector<vector<int>> matrix(m, vector<int>(n));
	vector<int> result_monoThread(m);
	int num_6 = 0;
	string text;
	Timer timer;

	for (int i = 0; i < m; i++)
		generate(matrix[i].begin(), matrix[i].end(), []() { return rand() % 24; });




#pragma omp parallel num_threads(m) shared(matrix) private(num_6, text)
	{
#pragma omp master 
		{
			text = "\nThread " + to_string(omp_get_thread_num()) + " - Sakhniuk Vitalina, 20 variant\n";
			cout << text.data();

		}
	}

	timer.start_Timer();
#pragma omp parallel num_threads(m) shared(matrix) private(num_6, text)	
	{
		vector<int> result_parallelThread(m);


#pragma omp for schedule(static, 6) //#pragma omp for schedule(dynamic, 4)
		for (int i = 0; i < m; i++)
		{
			auto num_6 = copy_if(matrix[i].begin(), matrix[i].end(), result_parallelThread.begin(), [](int a) {return a > 6; });
#pragma omp critical 
			{
				cout << "MAX element in " << i << "th row is " << *std::max_element(result_parallelThread.begin(), result_parallelThread.end()) << endl;
			}
		}
	}
	cout << "\nTime: " << timer.stop_Timer() << " milliseconds" << endl;

	timer.start_Timer();
	for (int i = 0; i < m; i++)
	{

		auto num_6 = copy_if(matrix[i].begin(), matrix[i].end(), result_monoThread.begin(), [](int a) {return a > 6; });
#pragma omp critical 
		{
			cout << "MAX element in " << i << "th row is " << *std::max_element(result_monoThread.begin(), result_monoThread.end()) << endl;
		}
	}
	cout << "\nTime: " << timer.stop_Timer() << " milliseconds" << endl;
}

void second_task()
{
#pragma omp parallel
	{
#pragma omp sections
		{
#pragma omp section
			{
				constexpr int sizeOfArr = 60;
				vector<int> arr(sizeOfArr);
				int mul = 1;
				generate(arr.begin(), arr.end(), []() { return rand() % 100 - 50; });


#pragma omp critical 
				{
					cout << "\nArray: ";
					copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, " "));
				}

#pragma omp critical 
				{
					cout << endl << "Sum = " << accumulate(arr.begin(), arr.end(), 0, [](int a, int b) {return (b > 0) ? a + b : a; }) << endl;

					for_each(arr.begin(), arr.end(), [&](int x) {if (x < 0) mul *= x; });
					cout << "Multiplication = " << mul << endl;
				}
			}
#pragma omp section
			{vector<vector<int>> matrix(6, vector<int>(8));


			for (int i = 0; i < matrix.size(); i++)
				generate(matrix[i].begin(), matrix[i].end(), []() { return (rand() % 100 - 50); });
#pragma omp critical 
			{
				cout << "\n\nArray:";
				for (int i = 0; i < matrix.size(); i++)
				{
					cout << endl;
					copy(matrix[i].begin(), matrix[i].end(), ostream_iterator<float>(cout, " "));
				}
			}

			cout << "\n\nArray after replacing:";
			for (int i = 0; i < matrix.size(); i++)
			{
				cout << endl;
				for_each(matrix[i].begin(), matrix[i].end(), [](int& n) { if (n < 0) n = 0; });
				copy(matrix[i].begin(), matrix[i].end(), ostream_iterator<float>(cout, " "));
			}
			}
		}
	}
}

int main()
{
	srand(time(NULL));

	first_task();
	second_task();

	system("pause");
	return 0;
}


