#include "bloomfilter.h"

typedef unsigned long ULONG;

ULONG BloomFilter::hashCode(const char* str,int n)
{
	ULONG seeds[] = {2, 6, 9, 11, 15, 20, 21, 23, 33, 40, 58}; //
	ULONG seed = seeds[n];
	ULONG hash = 0;

	while (*str)
	{
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF)% _mem; 
}

bool BloomFilter::add(const char* str)
{
	if(NULL == str)
		return false;
	if(exists(str))
		return false;
	for(int i=0;i<_k;++i)
	{
		ULONG hcode = hashCode(str,i);
		_bitset.set(hcode);
	}
	return true;
}

bool BloomFilter::exists(const char* str)
{
	bool result=false;
	if(NULL == str)
		return result;
	for(int i=0;i<_k;++i)
	{
		ULONG hc = hashCode(str,i);
		if(! _bitset.test(hc))
		{
			return result;
		}
	}
	return true;
}
