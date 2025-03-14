/**
Prime Number Generator

Author & Copyright (C) 2020 Johannes B. Steffens.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <vector>
#include "qprimes.h"

struct Defer {
	std::vector<void*> v;
	~Defer() {
		for( auto p: v) 
		{
			free(p);
		}
	}
	void operator()(void* p){ v.push_back(p); }
};

uint64_t get_prime(uint64_t min, uint64_t max)
{
	Defer defer;
	
	uint64_t val_min = min;
	uint64_t val_max = max;

	val_max = val_max < val_min ? val_min : val_max;

	uint64_t val_range = val_max - val_min;

	// sieving initial primes ...
	uint64_t smax  = sqrt( val_max ) + 1;
	uint64_t sbits = (  smax >> 1 ) + 1;
	size_t d0size = ( sbits >> 3 ) + 1;
	uint8_t* d0 = (uint8_t*)malloc( d0size );
	defer(d0);
	
	memset( d0, 0, d0size );

	d0[ 0 ] = 1;
	for( uint64_t n = 3; n * n < smax; n += 2 )
	{
		if( !( d0[ n >> 4 ] & ( 1 << ( ( n >> 1 ) & 7 ) ) ) )
		{
			for( uint64_t i = ( n * n ) >> 1; i < sbits; i += n )
			{
				d0[ i >> 3 ] |= ( 1 << ( i & 7 ) );
			}
		}
	}

	// output initial primes that overlap with test-range ...
	if( val_min <= 2 && val_max >= 2 )
	{
		return 2ull;
	}

	for( uint64_t i = val_min >> 2; i < sbits; i++ )
	{
		if( !( d0[ i >> 3 ] & ( 1 << ( i & 7 ) ) ) )
		{
			uint64_t prime = i * 2 + 1;
			if( prime >= val_min && prime <= val_max )
			{
				return prime;
			}
		}
	}

	// stepping through pages ...

	uint64_t pg_size_exp = 20;
	for( uint64_t i = 0; i < pg_size_exp; i++ )
	{
		if( ( 1ull << i ) > val_range )
		{
			pg_size_exp = i + 1;
			break;
		}
	}

	size_t d1size = 1ull << pg_size_exp;
	uint8_t* d1 = (uint8_t*)malloc( d1size );
	defer(d1);
	uint64_t pg_start = val_min >> pg_size_exp;
	uint64_t pg_end   = val_max >> pg_size_exp;
	for( uint64_t pg = pg_start; pg <= pg_end; pg++ )
	{
		for( uint64_t j = 0; j < d1size; j++ ) d1[ j ] = ( j & 1 ) ? 0 : 1;
		for( uint64_t i = 1; i < sbits; i++ )
		{
			if( !( d0[ i >> 3 ] & ( 1 << ( i & 7 ) ) ) )
			{
				uint64_t prime = i * 2 + 1;
				uint64_t start = ( prime - ( ( pg << pg_size_exp ) % prime ) ) % prime;
				for( uint64_t j = start; j < d1size; j += prime ) d1[ j ] = 1;
			}
		}

		for( uint64_t j = 0; j < d1size; j++ )
		{
			if( !d1[ j ] )
			{
				uint64_t prime = ( pg << pg_size_exp ) + j;
				if( prime > val_max ) break;
				if( prime > 1 && prime >= val_min )
				{
					return prime;
				}
			}
		}
	}
	return 0;
}
