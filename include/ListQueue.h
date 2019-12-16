/*=================USED AS IS FROM C++ EXERCISE ON PATIENT SIMULATOR =============================*/
#pragma once

#include <vector>
#include "Simulator.h"

class ListQueue : public OrderedSet {
private:
	std::vector<Comparable*>* elements;
public:
	ListQueue();
	void insert(Comparable* x);
	Comparable* removeFirst(); 
	Comparable* remove(Comparable* x); 
	int size();
	~ListQueue();
};


