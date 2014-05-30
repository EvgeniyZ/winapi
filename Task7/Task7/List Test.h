#pragma once
#include <iostream>
#include "Numeral.h"

//#include <vld.h>
//#include "Cities.h"

class List : public Numeral  // Закольцованный однонаправленный список
{
	CRITICAL_SECTION cs;
	struct ListElem 
	{		
		~ListElem ()
		{
			delete info;
		}
		ListElem *next;
		Numeral* info;
	};
	ListElem *head_;   // Указатель на голову списка

public:
	List ()
	{
		head_ = 0;	
		InitializeCriticalSection(&cs);
	}

	List (Numeral &element)
	{
		head_ = new ListElem;
		head_->info = element.GetCopy();
		head_->next = head_;
		InitializeCriticalSection(&cs);
	}

	~List ()
	{
		Make_Empty();	
		DeleteCriticalSection(&cs);
	}	

	List (const List &list)
	{
		head_ = new ListElem;
		ListElem *helper = head_;
		head_->next = head_;
		head_->info = list.head_->info->GetCopy();			
		ListElem *starter = list.head_->next;
		while (starter != list.head_)
		{
			this->Add(*starter->info);
			starter = starter->next;
		}
		InitializeCriticalSection(&cs);
	}

	List & operator =(List &list)
	{
		if ( this == &list) return *this;
		else
		Make_Empty ();			
		head_ = new ListElem;
		ListElem *helper = head_;
		head_->next = head_;
		head_->info = list.head_->info->GetCopy();	
		ListElem *starter = list.head_->next;
		InitializeCriticalSection(&cs);
		while (starter != list.head_)
		{			
			this->Add(*starter->info);
			starter = starter->next;
		}
		return *this;
	}

	void Add (Numeral &element)
	{
		EnterCriticalSection(&cs);
		ListElem *created = new ListElem;
		ListElem *helper = head_;		
		while (helper->next != head_)
		{
			helper = helper->next;
		}
		helper->next = created;
		created->next = head_;
		created->info = element.GetCopy();
		LeaveCriticalSection(&cs);
	}

	void Delete ()
	{		
		if (head_ == 0)
		{			
			return;
		}
		EnterCriticalSection(&cs);
		if (head_ == head_->next)
		{
			delete head_;
			head_ = 0;
		}			
		else
		{
			ListElem *helper = head_;
			while (helper->next != head_)
			{
				helper = helper->next;
			}
			ListElem *current =head_;
			while (current->next != helper)
			{
				current = current->next;
			}

			current->next = head_;
			delete helper;		
		}
		LeaveCriticalSection(&cs);
	}	

	void Make_Empty ()
	{
		if (head_ == 0)
		{
			return;
		}
		EnterCriticalSection(&cs);
		while (head_ != head_->next)
		{
			ListElem *current = head_->next;
			ListElem *helper = current->next;
			head_->next = helper;
			delete current;
		}	
		delete head_;
		head_ = NULL;
		LeaveCriticalSection(&cs);
	}	

	List* GetCopy () const
	{
		return new List(*this);
	}

	void Print () const 
	{
		if (head_ == 0)
		{
			return ;
		}

		ListElem *helper = head_;		
		while (helper->next != head_)
		{
			helper->info->Print();	
			helper = helper->next;				
		}
		helper->info->Print();
	}
};


