#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <iomanip>
#include "qprimes.h"

using namespace std;
using namespace std::chrono_literals;

uint64_t run = 0;
mutex m;

uint64_t p64()
{
	uint64_t prime = 0;
	random_device r;
	do {
		uint64_t start = 0;
		while(r.entropy() < 32);
		start = (uint64_t)r() << 32;
		while(r.entropy() < 32);
		start |= 0x8000000000000000 | (r() & 0xFFFFFF00);
		prime = get_prime(start,start+0x000000FF);
	} while(prime == 0);
	return prime;
}

void work(vector<uint64_t>* output)
{	
	while(true)
	{
		uint64_t p = p64();
		{
			lock_guard<mutex> lock(m);
			output->push_back(p);
		}
		if(!run) return;
	}
}

int main(int argc, char const *argv[])
{
	vector<uint64_t> primes;
	mutex m;
	size_t tnum = 12;
	if(argc > 1) tnum = atoi(argv[1]);
	ostream* outp = &cout;
	if(argc > 2) outp = new ofstream(argv[2]);
	ostream& out = *outp;
	
	vector<thread*> threads(tnum);
	for(size_t i = 0; i < tnum; ++i)
	{
		threads[i] = new thread(work, &primes);
	}
	size_t found = 0;
	run = 1; 	
	while(found <= 10)
	{
		this_thread::sleep_for(1000ms);
		{
			lock_guard<mutex> lock(m);
			if(primes.size() >= 20)
			{
				for(auto& e : primes)
				{
					out << hex << e << endl;
					found++;
				}
			}
		}
	}
	run = 0;
	for(size_t i = 0; i < tnum; ++i)
	{
		threads[i]->join();
	}
	lock_guard<mutex> lock(m);
	for(auto& e : primes)
	{
		out << hex << e << endl;
	}

	return 0;
}
