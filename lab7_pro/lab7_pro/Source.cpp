#include <cilk/cilk.h>
#include <cilk/reducer_max.h> 
#include <iostream>
#include <string>
#include <chrono>
#include <math.h>
#include <cilk/reducer.h>
#include <cilk/cilk_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;
cilk::reducer< cilk::op_max<int> > maxs;
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

void calcSumMono(int maxsum, vector<vector<int>> a, vector<int> s,int n) {
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

void calcSumCilk(vector<vector<int>> a, vector<int> s, int n) {
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
	//maxsum = -1000;

	//find maxs
	cilk_for( int i = 0; i < n; ++i)
	{
		maxs->calc_max(s[i]); 
	}
	cout << "Maximum value is " << maxs.get_value() << endl;
	
}


int main()
{
	constexpr int n = 3;
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
			cout << a[i][j]<<"\t";
		}
		cout << endl;
	}
	cout << "Sum of each diag: " << endl;

	timer.Start();
	calcSumMono(maxsum,a,s,n);
	timer.Stop();
	cout << "\nMono-thread time: " << timer.elapsedMilliseconds() << endl;
	timer.Start();
	cout << "Sum of each diag: " << endl;

	calcSumCilk(a, s, n);
	timer.Stop();
	cout << "Parallel time: " << timer.elapsedMilliseconds() << endl;
	
	return 0;
}



