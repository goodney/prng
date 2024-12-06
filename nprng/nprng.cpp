#include <cstdint>
#include <cstddef>
#include <cstdio>
#include "crc.h"

//64-bit prime numbers from OpenSSL
uint64_t a[4] = {0xD52F40791AC224C5,
				 0xE4A8BA66EC2BB68F,
				 0xDF4FD3DF951C4A91,
				 0xC3B6075841107027};
				 
uint64_t c[4] = {0xD2A91E8D2280074B,
				 0xEAA15A2052CFD811,
			 	 0xFF9BB2DCEEA9843B,
			 	 0xC0E3318FE7E4F00B};
				  
//some random bytes on dev-day
//seems to work well with any initial state	
		  
uint64_t state[4] = {0xef6c2fd2a9ef229f, 
					 0x4e429013125c55b7,
					 0x42018b271515e037, 
					 0xd3e8d7f96469651a};

//uint64_t state[4] = {1,2,3,4};
//uint64_t state[4] = {0,0,0,0};

void nprng_seed(uint64_t seed[4])
{
	state[0] ^= seed[0];
	state[1] ^= seed[1];
	state[2] ^= seed[2];
	state[3] ^= seed[3];
	
}

size_t next = 0;

uint64_t nprng_next()
{
	state[next] = state[next] * a[next] + c[next];
	uint64_t crc = crc64(0, (const unsigned char*)state, 32);
	next = crc % 4;
	return crc;
}

#ifdef __TEST__
int main(int argc, char* argv[])
{
	
	while(true)
	{
		uint64_t r = nprng_next();
		fwrite (&r, 1, 8, stdout); 
	}
}
#endif