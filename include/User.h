#ifndef SUPERCOMPUTERSIMULATION_USER_H
#define SUPERCOMPUTERSIMULATION_USER_H

#include <unordered_set>
#include "AbstractScheduler.h"
#include "HPCParameters.h"
/*
* Generate a stream of jobs for 8.0 time units.
*/

class AbstractScheduler;
/**
 * User class specify  the behavior of Users.
 * Users events corresponds to a user creating a new job and trying to submit it.
 * it will reschedule itself if the job required budget is not above it budget left.
 */
class User : public Event {
protected:
    /**
     * Individual budget of the user
     */
    double budget = 0;
    /**
     * Maximum of nodes that the user can ask for at the same time
     * default value in the totalNumberOfNode defined by HPC parameters
     */
    int instantaneousMaxNumberOfNodes = HPCParameters::TotalNumberOfNodes;
    /**
     * Keep tracks of how many nodes the users as ask for jobs in the waiting queues and being executed
     */
    int currentlyUsedNumberOfNodes = 0;
    /**
     * Reference to the scheduler for submitting jobs
     */
    AbstractScheduler *scheduler;
    /**
     * Static variable used as counter to give a unique id to each User
     */
    static int numOfUsers;
    /**
     * User unique Id
     */
    int userId;
    /**
     * mean time between to different jobs created by the user
     */
    double meanTimeToNextJob = 12;// by default a user will try to generate a job every 12 hours

    /**
     * Permissions of the users for generating jobs of types : small,medium,large,huge,gpu
     */
    bool permissions[5]={0, 0, 0, 0, 0};


public:
    /**
     * Return the mean time between to different jobs created by the user
     * @return
     */
    double getMeanTimeToNextJob() const;

    /**
     * Set mean time between to different jobs created by the user
     * @param meanTimeToNextJob
     */
    void setMeanTimeToNextJob(double meanTimeToNextJob);

    /**
     * Return the user Id
     * @return
     */
    int getUserId() const;

    /**
     * set userId and Increment the number of user
     */
    User();

    User(double meanTimeBetweenToJobs);

    User(double meanTimeBetweenToJobs, double firstJobTime);

    User(const User &g) = delete;

    User &operator=(const User &g) = delete;

    /**
     * Executing this events means creating randomly a new job and trying to submit it.
     * the user will reschedule itself if the job required budget is not above it budget left.
     *
     * @param simulator
     */
    void execute(AbstractSimulator *simulator);

    /**
     * set the scheduler for submitting jobs
     * @param scheduler
     */
    void addScheduler(AbstractScheduler *scheduler) { this->scheduler = scheduler; };

    /**
     * return the budget left for this user
     * @return
     */
    virtual double budgetLeft();

    /**
     * remove a certain amout from the budget
     * @param amountToRemove
     */
    virtual void removeFromBudget(double amountToRemove);

    /**
     * Decrease the number of job currently used by this user
     * @param numberOfNodes
     */
    void reduceNumberOfCurrentlyUsedNodeBy(int numberOfNodes) {
        currentlyUsedNumberOfNodes -= numberOfNodes;
    };

    /**
     * Set the permissions of this user.
     */
    void setPermission (bool small,bool medium,bool large,bool huge,bool gpu);

};

#endif //SUPERCOMPUTERSIMULATION_USER_H
