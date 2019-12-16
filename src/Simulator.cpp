#include "../include/Simulator.h"
#include <cmath>

Event::Event(double time) { this->time = time; }

bool Event::lessThan(Comparable* y) {
	Event* e = (Event*)y;
	return time < e->time;
}

void Event::execute(AbstractSimulator* simulator) 
{ 
	std::cout << std::endl <<"Time is " << convertTime(simulator->now()) << "\n";
}

double Event::getTime() { return time; }



Simulator::Simulator() { time = 0.0; }

double Simulator::now() {
	return time;
}

void Simulator::doAllEvents() {
	Event* e;
	while ((e = (Event*)events->removeFirst()) != 0) {
		time = e->getTime();
		e->execute(this);
	}
}

// convert the time in seconds to hrs, mins, secs
string convertTime(double t) {
    // current time value is decimal in hours, so multiply by 3600 to get seconds and round
    int time = static_cast<int>(round(t * 3600));

    // convert seconds to hrs, mins, secs
    int hour = time / 3600;
    time = time % 3600;
    int min = time / 60;
    time = time % 60;
    int sec = time;

    string s = to_string(hour) + "hrs:" + to_string(min) + "mins:" + to_string(sec) + "secs";
    return s;
}