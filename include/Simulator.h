/*=================USED AS IS FROM C++ EXERCISE ON PATIENT SIMULATOR =============================*/
#pragma once

#include "AbstractSimulator.h"

std::string convertTime(double time);

class Event : public AbstractEvent {
protected:
	double time;
public:
	Event(double time = 0);
	bool lessThan(Comparable* y); 
	void execute(AbstractSimulator* simulator); 
	double getTime();
};


class Simulator : public AbstractSimulator {
protected:
	double time;  // time into the simulation due to event processing
public:
	Simulator();
	double now();
	void doAllEvents();
};
