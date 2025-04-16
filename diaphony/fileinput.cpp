#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

vector<vector<double> > fileinput(istream& in)
{
	//find the first non-comment line #
	string line;
	do {
		getline(in, line);
		if(in.fail())
		{
			throw std::runtime_error("File ended while looking for first data line.");
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
		throw std::runtime_error("File ended with no columns detected.");
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
				throw std::runtime_error("Error reading on data line " + to_string(rows));
			}
			v.push_back(t);
		}
		data.push_back(v);
	} while( getline(in, line));
	return data;
}
