#include <random>
#include <iostream>
//#include <iomanip>
#include "qprimes.h"

using namespace std;

static const uint64_t C = 0xbea225f9eb34556dUL;
//static const uint64_t C = 0x9E6C63D0676A9A99UL;

static inline uint64_t ror64(uint64_t v, int r) {
	return (v >> r) | (v << (64 - r));
}

/*
static inline uint64_t mix(uint64_t x) {
  // ror64(a, r) is a 64-bit rotation of a by r bits.
  x ^= ror64(x, 25) ^ ror64(x, 47);
  x *= 0x9E6C63D0676A9A99UL;
  x ^= x >> 23 ^ x >> 51;
  x *= 0x9E6D62D06F6A9A9BUL;
  x ^= x >> 23 ^ x >> 51;

  return x;
}
*/

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


inline uint64_t mix_stream(uint64_t h, uint64_t x)
{
	h += mix(x);
	h *= C;
	return h;
}

inline uint64_t mix_stream(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	h = mix_stream(h,a);
	h = mix_stream(h,b);
	h = mix_stream(h,c);
	h = mix_stream(h,d);
	return h;
}

uint64_t primes[] = {0x915be6ff8890b81f,
	0x959977aaa3770013,
	0x9d0939272f402f01,
	0xa17670985ab5810d,
	0xa8ced0cbddfa9da1,
	0xb9914721d3b44e05,
	0xba688d340f84772f,
	0xbcc9a7994ec90433,
	0xc448db3ebcb96e4f,
	0xc4caa6f7039a2777,
	0xdabf0686736bbf23,
	0xf33c93b1c21dad0d,
	0x9ebf8cd4d79b3c0b,
	0xb2c3a670c20ad261,
	0xd132882bfa36f81f,
	0x8e9a08ca10b04407,
	0xcf106d24f4e46e17,
	0x9ae86db7e4775a17,
	0xa8529b33d5c72809,
	0xb4cf795e00b58a0b,
	0xd04aa02b0160f15d,
	0xc20ea455397f2919,
	0xb97d081483426021};
	
size_t idx = 8;
uint64_t p64()
{
	return primes[idx++];
}

class pararng {
	public:
		pararng();
		uint64_t operator()();
	private:
		uint64_t state[2];
		uint64_t a[2];
		uint64_t c[2];
		uint64_t m[2];
};

pararng::pararng()
{
	a[0] = p64();
	a[1] = p64();
	c[0] = p64();
	c[1] = p64();
	m[0] = p64();
	state[0] = p64();
	state[1] = p64();
}

uint64_t pararng::operator()() {
	//xn+1 = xn*a + c (mod m)
	__uint128_t S[2];
	S[0] = (__uint128_t)state[0]*a[0] + c[0];
	S[1]  = (__uint128_t)state[1]*a[1] + c[1];
	uint64_t* s = (uint64_t*)S;
	uint64_t x = mix_stream(0, s[0], s[1], s[2], s[3]);
	state[0] = S[0] % (__uint128_t)m[0];
	state[1] = S[1];
	return mix(x);
}

int main(int argc, char* argv[])
{
	pararng p;
	while(1)
	{
		uint64_t r = p();
		fwrite (&r, 1, 8, stdout);
	}
}
