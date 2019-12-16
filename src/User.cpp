#include "../include/User.h"

User::User() {
    time = Random::exponential(meanTimeToNextJob);
    userId = ++numOfUsers;
}

User::User(double meanTimeBetweenTwoJobs) : meanTimeToNextJob(meanTimeBetweenTwoJobs) {
    time = Random::exponential(meanTimeToNextJob);
    userId = ++numOfUsers;
}

User::User(double meanTimeBetweenToJobs, double firstJobTime) : meanTimeToNextJob(meanTimeBetweenToJobs),
                                                                Event(firstJobTime) {
    userId = ++numOfUsers;
}

int User::numOfUsers = 0;

/**
Inspired by Generator
 A user is generating Jobs for the HPC
*/
void User::execute(AbstractSimulator *simulator) {
    Event::execute(simulator);
    AbstractJob *job = CreateRandomJob(permissions);
    job->generateRandomRequirements();
    if (currentlyUsedNumberOfNodes + job->getNumberOfNodes() <= instantaneousMaxNumberOfNodes) {
        // keep the simulator going until next planned job is too large
        //TODO assumption + GPU NODES
        double jobCost;
        if (job->isGpuJob()) {
            jobCost = job->getExecutionDuration() * HPCParameters::costOneHourOneGPUNode * job->getNumberOfNodes();
        } else {
            jobCost = job->getExecutionDuration() * HPCParameters::costOneHourOneNode * job->getNumberOfNodes();
        }

        if (budgetLeft() - jobCost >= 0) {
            job->setSubmittingTime(time);
            job->setUser(this);
            job->insertIn(simulator, scheduler);// insert the job int the scheduler

            removeFromBudget(jobCost);
            time += Random::exponential(meanTimeToNextJob);
            simulator->insert(this);
            currentlyUsedNumberOfNodes += job->getNumberOfNodes();

            std::cout << job->getType() << "job " << job->getId() << " submitted at time " << convertTime(time)
                      << " by User " << userId
                      << "\n";
            std::cout << "Job " << job->getId() << " requires " << job->getNumberOfNodes() << " nodes\n";
            std::cout << "User " << userId << " is using " << currentlyUsedNumberOfNodes << " out of "
                      << instantaneousMaxNumberOfNodes << "\n"
                      << " Budget left " << convertTime(budgetLeft()) << "\n";
        } else {
            std::cout << "User " << userId << " has not enough budget left for is next job. Budget left : "
                      << budgetLeft() << " / Next Job : " << convertTime(job->getExecutionDuration()) << " on "
                      << job->getNumberOfNodes()
                      << " Nodes \n";
        }
    } else {
        time += Random::exponential(12);
        std::cout << "User " << userId << " has not enough instantaneous nodes for this job. \n"
                  << "User will try to submit an other job at :" << convertTime(time) << ". \n"
                  << "New Nodes required : " << job->getNumberOfNodes() << "/ currently used nodes :"
                  << currentlyUsedNumberOfNodes << "/ max :" << instantaneousMaxNumberOfNodes << "\n"
                  << " Budget left " << convertTime(budgetLeft()) << "\n";;

        simulator->insert(this);
    }

}

double User::budgetLeft() {
    return budget;
}

int User::getUserId() const {
    return userId;
}

void User::removeFromBudget(double amountToRemove) {
    budget -= amountToRemove;
}

double User::getMeanTimeToNextJob() const {
    return meanTimeToNextJob;
}

void User::setMeanTimeToNextJob(double meanTimeToNextJob) {
    User::meanTimeToNextJob = meanTimeToNextJob;
}

void User::setPermission(bool small, bool medium, bool large, bool huge, bool gpu) {
    permissions[0] = small;
    permissions[1] = medium;
    permissions[2] = large;
    permissions[3] = huge;
    permissions[4] = gpu;
}


