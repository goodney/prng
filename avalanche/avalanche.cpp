#include <random>
#include <iostream>
#include "crc.h"

using namespace std;

static const uint64_t C = 0xbea225f9eb34556d;

inline uint64_t mix(uint64_t x) {
	x ^= x >> 32;
	x *= C;
	x ^= x >> 29;
	x *= C;
	x ^= x >> 32;
	x *= C;
	x ^= x >> 29;
	return x;
}

int main(int argc, char* argv[])
{
	double vals[64][64] = {0};
	int N = 1024;
	random_device r;
	for(size_t x = 0; x < N; x++)
	{
		while(r.entropy() < 32);
		uint64_t val = (uint64_t)r() << 32;
		while(r.entropy() < 32);
		val |= r();
		//uint64_t hash = crc64(0, (const unsigned char*)&val, 8);
		uint64_t hash = mix(val);
		for(size_t i = 0; i < 64; i++)
		{
			uint64_t new_val = val ^ (1 << i);
			//uint64_t new_hash = crc64(0, (const unsigned char*)&new_val,8);
			uint64_t new_hash = mix(new_val);
			uint64_t diff = hash ^ new_hash;
			for(size_t j = 0; j < 64; j++)
			{
				if(diff & 1) vals[i][j]++;
				diff = diff >> 1;
			}
		}
	}
	for(size_t i = 0; i < 64; i++)
	{
		for(size_t j = 0; j < 64; j++)
		{
			cout << vals[i][j]/N << " ";
		}
		cout << endl;
	}
}