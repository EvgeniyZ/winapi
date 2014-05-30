#pragma once

#include "windows.h"
#include <iostream>
class Numeral 
{
	static HANDLE hHeap; 
	static int heapCounter;
public:
	Numeral();
	virtual ~Numeral();
	virtual Numeral* GetCopy() const = 0;	

	friend std::ostream& operator<< (std::ostream & out, Numeral &city)
	{
		city.Print();
		return out;
	}
	
	virtual void Print () const = 0;	

	void *operator new(size_t size);

	void operator delete(void *p);

	void *operator new[](size_t size);

	void operator delete[](void *p);
};