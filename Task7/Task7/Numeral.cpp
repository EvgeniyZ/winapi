#include "stdafx.h"
#include "Numeral.h"
#include <iostream>

HANDLE Numeral::hHeap = 0;
int Numeral::heapCounter = 0;

Numeral::Numeral() {}

Numeral::~Numeral() {}

void * Numeral::operator new(size_t size){
	void *p;
	if (heapCounter++ > 0) 	
		p = HeapAlloc(hHeap, HEAP_NO_SERIALIZE, size);				
	else
	{
		hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0, 0);
		p = HeapAlloc(hHeap, HEAP_NO_SERIALIZE, size);
	}		
	if (p == NULL)
		throw std::bad_alloc();
	return p;
}

void Numeral::operator delete(void *p) {
	if (--heapCounter != 0) {
		HeapFree(hHeap, HEAP_NO_SERIALIZE, p);
	}	
	else {
		HeapDestroy(hHeap);
	}
}

void * Numeral::operator new[](size_t size) {	
	return operator new(size);
}

void Numeral::operator delete[](void *p) {
	operator delete(p);
}