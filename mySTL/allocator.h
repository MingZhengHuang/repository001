#pragma once
#include <cstdio>
#include <iostream>
namespace hmz
{
template <class T>
class allocator
{
public:
	typedef T  value_type;
	typedef T* pointer;

	pointer allocate(int n)
	{
		return new T[n];
	}
	void deallocate(pointer p)
	{
		delete []p;
	}
};


}
