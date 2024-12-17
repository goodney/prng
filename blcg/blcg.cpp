#include <random>
#include <iostream>
#include <iomanip>
#include "qprimes.h"

using namespace std;

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

int main(int argc, char* argv[])
{
	random_device r;
	while(r.entropy() < 32);
	uint64_t state = (uint64_t)r() << 32;
	while(r.entropy() < 32);
	state |= r();
	uint64_t a = p64();
	uint64_t c = p64();
	cerr << hex << state << " " << a << " " << c << endl;
	size_t total = 0;
	while(total < 8*10000000)
	{
		state = state * a + c;
		fwrite (&state, 1, 8, stdout);
		total += 8;
	}
}