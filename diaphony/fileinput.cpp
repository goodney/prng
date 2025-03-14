#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{
	istream& in = *((argc > 1) ? new ifstream(argv[1]) : &cin);
	//find the first non-comment line #
	string line;
	do {
		getline(in, line);
		if(in.fail())
		{
			cerr << "File ended while looking for first data line." << endl;
			exit(1);
		}
	} while (line.size() > 0 && line[0] == '#');
	//count the columns
	size_t cols = 0;
	double t;
	stringstream ss(line);
	while(ss >> t)
	{
		++cols;
	}
	if(cols == 0)
	{
		cerr << "File ended with no columns detected." << endl;
		exit(1);
	}
	size_t rows = 0;
	vector<vector<double> > data;
	do {
		if(line.size() == 0) continue;
		++rows;
		stringstream ss(line);
		vector<double> v;
		for(size_t i = 0; i < cols; ++i)
		{
			ss >> t;
			if(ss.fail())
			{
				cerr << "Error reading on data line " << rows << endl;
				exit(1); 
			}
			v.push_back(t);
		}
		data.push_back(v);
	} while( getline(in, line));
	cout << "Read input file with " << rows << " rows and " << cols << " columns." << endl;
}
