#include <cilk/cilk.h>
#include <cilk/reducer_ostream.h> 
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
cilk::reducer<cilk::op_ostream> r(std::cerr);
using namespace std;

int main()
{
	int n,z;
	cout << "Please, enter the size of vector" << endl;
	cin >> n;
	vector <int> s(n);
	for (int i = 0; i < s.size(); i++) {
		cout << endl << "Enter the " << i << "th element of vector: ";
		cin >> s[i];
	}
	 
	cout << "Please, enter the number for multiplication" << endl;
	cin >> z;

	cilk_for (int i = 0; i < s.size(); i++) {
		s[i] *= z;
	}
	cout << "Vector after calculating" << endl;

	cilk_for (int i = 0; i < s.size(); i++) {
		*r << s[i]<<"\t";
	}


	

	return 0;
}