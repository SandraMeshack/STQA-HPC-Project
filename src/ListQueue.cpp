#include "../include/ListQueue.h"
#include <cmath>

// a list queue implementation using a vector for storage

ListQueue::ListQueue()  {
	elements = new std::vector<Comparable*>();
}

void ListQueue::insert(Comparable* x) {
	unsigned int i = 0;
	while (i < elements->size() && ((*elements)[i])->lessThan(x)) {
		i++;
	}
	auto it = elements->begin();
	(*elements).insert(it + i, x);
	i = 0;
}

Comparable* ListQueue::removeFirst() {
	if (elements->size() == 0) return 0;
	Comparable* x = elements->front();
	std::vector<Comparable*>::iterator it = elements->begin();
	elements->erase(it);
	return x;
}

Comparable* ListQueue::remove(Comparable* x) {
	for (unsigned int i = 0; i < elements->size(); i++) {
		if (elements->at(i) == x) {
			Comparable* y = elements->at(i);
			std::vector<Comparable*>::iterator it = elements->begin() + i;
			elements->erase(it);
			return y;
		}
	}
	return 0;
}

int ListQueue::size() {
	return elements->size();
}

ListQueue::~ListQueue() { 
	delete elements;
}