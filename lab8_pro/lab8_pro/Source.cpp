#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <iostream>
#include <string>
#include <chrono>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;
class Timer
{
public:
	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void Stop()
	{
		end = std::chrono::high_resolution_clock::now();
	}
	double elapsedMilliseconds()
	{
		duration = end - start;
		return duration.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float, milli> duration;
};

void calcSumMono(int maxsum, vector<vector<int>> a, vector<int> s, int n) {
	//calculate sum of each diag
	for (int j = 0; j < (2 * n); j++) s[j] = 0;
	for (int i = 0; i < n; i++)
	{
		for (size_t j = 0; j + i < n; j++)
		{

			s[j] += a[i][i + j];
			s[n + j] += a[i + j][i];

		}
	}
	for (int j = 1; j < n; j++) cout << s[j] << "\n";
	for (int j = n + 1; j < (2 * n); j++) cout << s[j] << "\n";
	maxsum = -1000;


	//find maxsum
	for (int j = 1; j < n; j++)
		if (maxsum < s[j])
			maxsum = s[j];


	cout << "Max sum = " << maxsum << endl;
}

void calcSumTBB(int maxsum, vector<vector<int>> a, vector<int> s, int n) {
	//calculate sum of each diag
	for (int j = 0; j < (2 * n); j++)
	{
		s[j] = 0;
	}
	for (int i = 0; i < n; i++)
	{
		for (size_t j = 0; j + i < n; j++)
		{

			s[j] += a[i][i + j];
			s[n + j] += a[i + j][i];

		}
	}
	for (int j = 1; j < n; j++) cout << s[j] << "\n";
	for (int j = n + 1; j < (2 * n); j++) cout << s[j] << "\n";
	

	//find maxsum
	tbb::parallel_for(tbb::blocked_range<int>(0, s.size()), [&](tbb::blocked_range<int> r)
		{
			for (int i = r.begin(); i < r.end(); i++) {
				if (maxsum < s[i])
					maxsum = s[i];
			}
		});


	cout << "Max sum = " << maxsum << endl;

}


int main()
{
	constexpr int n = 5;
	vector<vector<int>> a(n, vector<int>(n));
	vector <int> s(n * 2);

	Timer timer;
	int  maxsum = 0, sum = 0;
	srand(time(0));
	//generate matrix
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			a[i][j] = rand() % 10;

	//print matrix
	cout << "Matrix:" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << a[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "Sum of each diag: " << endl;

	timer.Start();
	calcSumMono(maxsum, a, s, n);
	timer.Stop();
	cout << "\nMono-thread time: " << timer.elapsedMilliseconds() << endl;
	timer.Start();
	cout << "Sum of each diag: " << endl;

	calcSumTBB(maxsum, a, s, n);
	timer.Stop();
	cout << "Parallel time: " << timer.elapsedMilliseconds() << endl;

	return 0;
}