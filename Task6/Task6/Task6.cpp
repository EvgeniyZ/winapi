// Task6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <process.h>

struct Partition {	
	int _begin;
	int _end;
	int** _matrix;
	int* _resolve;
	int _matrixSize;	
	int _threadsCount;

	Partition()
    {
		_resolve = 0;
		_matrix = 0;
		_matrixSize = 0;		
		_end = 0;
		_begin = 0;
		_threadsCount = 0;
	}
};

int** createMatrix(int size)
{
	int** pmatr = new int*[size];
	for (size_t i = 0; i < size; i++) {
		try {
			pmatr[i] = new int[size];
		}
		catch (const std::bad_alloc &)
		{
			for (size_t j = 0; j < i; j++)
			{
				delete[]pmatr[j];
			}
			delete[]pmatr;
			throw;
		}
	}

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			pmatr[i][j] = 1;
		}
	}
	return pmatr;
}

int* createMassive(int size) 
{
	int* massive = new int[size];
	for (size_t i = 0; i < size; i++)
	{
		massive[i] = 0;
	}
	return massive;
}

int* sumRows(int** matrix, int* sumMatrixRows, int size)
{	
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			sumMatrixRows[i] += matrix[i][j];
		}
	}
	return sumMatrixRows;
}

int* sumColumns(int** matrix, int* sumMatrixColumns, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			sumMatrixColumns[i] += matrix[j][i];
		}
	}
	return sumMatrixColumns;
}

/*int* partitionCallSum(Partition Partition) {
	for (size_t i = 1; i != Partition._threadsCount; i++)
	{
		threadSmartSumColumns(Partition._matrix, Partition._resolve, (Partition._matrixSize * i) / (Partition._threadsCount));
	}
	return Partition._resolve;
}*/

void threadSmartSumColumns(Partition partition)
{
	int value;
	int index;
	for (size_t i = partition._begin; i < partition._end; i++)
	{
		for (size_t j = 0; j < partition._matrixSize; j++)
		{
			partition._resolve[j] += partition._matrix[i][j];
		}
	}	
}

int* smartSumColumns(int** matrix, int* sumMatrixColumns, int size)
{
	int value;
	int index;
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			sumMatrixColumns[j] += matrix[i][j];
		}
	}
	return sumMatrixColumns;
}

int* summaryResolve(Partition partition[])
{
	int* resolve = createMassive(partition[0]._matrixSize);
	for (size_t i = 0; i < partition[0]._threadsCount; i++)
	{
		for (size_t j = 0; j < partition[0]._matrixSize; j++)
		{
			resolve[j] += partition[i]._resolve[j];
		}
	}
	return resolve;
}

DWORD WINAPI ThreadFunction(LPVOID lpV)
{	
	Partition * pPar = (Partition*)lpV;
	threadSmartSumColumns(*pPar);
	return 0;	
}


int _tmain(int argc, _TCHAR* argv[])
{
	// TODO ÑÄÅËÀÒÜ ÍÀ ÊÀÆÄÓÞ ÑÒÐÓÊÒÓÐÓ PARTITION ÑÂÎÉ ÌÀÑÑÈÂ !!! È ÏÎÒÎÌ ÏÐÎÑÑÓÌÈÐÎÂÀÒÜ È ÏÎËÓ×ÈÒÜ ÈÑÊÎÌÎÅ. 
	// À ÒÀÊ ÏÎËÓ×ÀÅÒÑß ÝÔÔÅÊÒ ÃÎÍÎÊ
	CONST INT size_ = 10000;
	CONST INT threadsCount = 2;
	DWORD dwStartTime;
	DWORD dwEndTime;
	DWORD dwTimeOnRows;
	DWORD dwTimeOnColumns;
	DWORD dwStartTime2;
	DWORD dwEndTime2;
	DWORD dwTimeOnRows2;
	DWORD dwTimeOnColumns2;
	DWORD dwStartTime3;
	DWORD dwEndTime3;
	DWORD dwTimeOnRows3;
	DWORD dwTimeOnColumns3;
	int** matrix = createMatrix(size_);	
	int* sumMatrixRows = createMassive(size_);
	int* sumMatrixColumns = createMassive(size_);
	int* resolve;
	int partSize = size_ / threadsCount;
	HANDLE hThreads[threadsCount];
	Partition threadsPartition[threadsCount];

	for (size_t i = 0; i < threadsCount; i++)
	{
		threadsPartition[i]._matrix = matrix; 
		threadsPartition[i]._matrixSize = size_;
		threadsPartition[i]._resolve = createMassive(size_);
		threadsPartition[i]._begin = partSize * i;
		threadsPartition[i]._end = threadsPartition[i]._begin + partSize;	
		threadsPartition[i]._threadsCount = threadsCount;
	}	

	typedef unsigned int(_stdcall *THREADFUNC)(void*);
	dwStartTime = GetTickCount();
	for (size_t i = 0; i < threadsCount; i++)
	{		
		hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, (THREADFUNC)ThreadFunction, &threadsPartition[i], 0, NULL);
	}
	WaitForMultipleObjects(threadsCount, hThreads, TRUE, 5000);
	
	dwEndTime = GetTickCount();	
	dwTimeOnColumns = dwEndTime - dwStartTime;
	resolve = summaryResolve(threadsPartition);
	dwStartTime2 = GetTickCount();
	sumMatrixRows = sumRows(matrix, sumMatrixRows, size_);
	dwEndTime2 = GetTickCount();
	dwTimeOnRows2 = dwEndTime2 - dwStartTime2;

	for (size_t i = 0; i < size_; i++)
	{
		std::cout << resolve[i];
		std::cout << std::endl;
	}

	std::cout << "TIME WORKING WITH THREADS       ";
	std::cout << dwTimeOnColumns << std::endl;

	std::cout << "TIME WORKING WITHOUT THREADS     ";
	std::cout << dwTimeOnRows2;

	return 0;
}

