#pragma once
#ifndef _BOOMFILTER_H_
#define _BOOMFILTER_H_


#include <cmath>
#include "bitcontainer.h"

class BloomFilter
{
public:
	typedef unsigned long ULONG;
public:
	BloomFilter(const ULONG max_elem_num=1000000,const double err_rate=0.01):
	  _bitset(BitContainer((ULONG)(log(1/err_rate)*max_elem_num*2))),
      _mem((ULONG)(log(1/err_rate)*max_elem_num*2))
	  {
		  _k=(int)(0.7*_mem/max_elem_num);
	  }
	bool add(const char* str);
	bool exists(const char* str);
	ULONG hashCode(const char* str,int n);
private:
	BitContainer _bitset;//a bit array
	int _k;//number of hash functions to be used
	ULONG _mem;//bits of memory need in bitContainer
};

#endif