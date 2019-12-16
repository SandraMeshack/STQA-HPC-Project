//
// Created by inao on 13/12/2019.
//

#ifndef SUPERCOMPUTERSIMULATION_WEEKENDEVENT_H
#define SUPERCOMPUTERSIMULATION_WEEKENDEVENT_H


#include "Simulator.h"
#include "AbstractScheduler.h"

/**
 * Weekend begin events happens at Friday 5PM
 */
class WeekendBegin : public Event {
private:
    /**
     * scheduler for giving orders to
     */
    AbstractScheduler *scheduler;

    const double numberOfHoursInAWeek = 168;
    const double numberOfHoursBeforeWeekEnd = 104;

public:
    /**
     * Executing this event send the message to the scheduler to try to run as
     * many Huge jobs as possible
     * Then it will reschedule itself if there is still jobs to execute or events
     * planned (except for weekend End)
     * @param simulator
     */
    void execute(AbstractSimulator *simulator) override;
    WeekendBegin(AbstractScheduler *scheduler);
};

/**
 * Weekend begin events happens at Monday 9AM
 */
class WeekendEnd : public Event {
private:
    /**
    * scheduler for giving orders to
    */
    AbstractScheduler *scheduler;
    const double numberOfHoursInAWeek = 168;
    const double numberOfHoursBeforeWeekEnd = 168;
public:
    /**
     * Executing this event send the message to the scheduler to try to run as
     * many jobs as possible according to priorities
     * Then it will reschedule itself if there is still jobs to execute or events
     * planned
     * @param simulator
     */
    void execute(AbstractSimulator *simulator) override;


    WeekendEnd(AbstractScheduler *scheduler);
};



#endif //SUPERCOMPUTERSIMULATION_WEEKENDEVENT_H
