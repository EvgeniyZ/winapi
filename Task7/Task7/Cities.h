#pragma once

#include <iostream>
//#include <vld.h>
#include "Numeral.h"
#include "Heap.h"

class City : public Numeral
{
	char *title_;
	int quantity_;	

public:

	City ()
		:Numeral()
	{	
		title_ = 0;			
		quantity_ = 0;
	}

	City (char * name, int number)
		:Numeral()
	{
		if (!name)
			name = "";		
		title_ = new char [strlen(name)+1];				
		strcpy(title_,name);
		quantity_ = number;
	}

	virtual ~City ()
	{		
		delete []title_;		
	}

	City (const City &city)
	{
		title_ = new char [strlen(city.title_)+1];		
		strcpy(title_,city.title_);			
		quantity_ = city.quantity_;
	}

	City & operator=(const City &city)
	{
		if (this == &city)
		{
			return *this;
		}
		delete [] title_;
		title_ = new char [strlen(city.title_)+1];
		strcpy(title_,city.title_);
		quantity_ = city.quantity_;
		return *this;
	}	

	City* GetCopy() const
	{
		return new City(*this);
	}

	void Print () const 
	{
		std::cout<<title_;
		std::cout<<quantity_;
		std::cout<<std::endl;
	}	
};