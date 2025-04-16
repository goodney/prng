#include <iostream>
#include <cstdlib>
#include "fileinput.h"
#include "diaphony.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<vector<double> > data = fileinput(cin);
	double diaphony = diaphony_compute(data);
	cout << "The diaphony of these points is: " << diaphony << endl;
	return 0;
}