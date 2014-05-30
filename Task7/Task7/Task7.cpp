// Task7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <process.h>
#include "List Test.h"
#include "Cities.h"

struct Partition {	
	List * list;	
	City * city;
};

DWORD WINAPI ThreadFunction(LPVOID lpV)
{
	Partition * pPar = (Partition*)lpV;
	pPar->list->Add(*pPar->city);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CONST INT threadsCount = 2;
	HANDLE hThreads[threadsCount];
	Partition threadsPartition[threadsCount];
	City washington("Washington", 10000);
	List * listTest = new List(washington);	
	City * london = new City("London", 123456);
	City * omsk = new City("Omsk", 5678);
	for (size_t i = 0; i < threadsCount; i++)
	{
		threadsPartition[i].list = listTest;
		if (i % 2 == 0)
			threadsPartition[i].city = london;
		else
			threadsPartition[i].city = omsk;
	}
	typedef unsigned int(_stdcall *THREADFUNC)(void*);
	for (size_t i = 0; i < 100; i++)
	{
		if (i % 2 == 0)
			hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, (THREADFUNC)ThreadFunction, &threadsPartition[0], 0, NULL);
		else
			hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, (THREADFUNC)ThreadFunction, &threadsPartition[1], 0, NULL);
	}
	WaitForMultipleObjects(threadsCount, hThreads, TRUE, 5000);
	listTest->Print();
	return 0;
}

