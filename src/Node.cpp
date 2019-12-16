#include "../include/Node.h"
#include <cmath>
#include "../include/AbstractScheduler.h"
#include "../include/User.h"
#include "../include/AbstractJob.h"
#include "../include/ListQueue.h"






Node::Node() : Event() {
    jobBeingExecuted = nullptr;
}

Node &Node::addScheduler(AbstractScheduler *scheduler) {
    this->scheduler = scheduler;
    return *this;
}



void Node::execute(AbstractSimulator *HPCsimulator) {
    Event::execute(HPCsimulator);
    printMessage();
    (jobBeingExecuted->getUser())->reduceNumberOfCurrentlyUsedNodeBy(1);
    jobBeingExecuted->setCompletionTime(HPCsimulator->now());
    jobBeingExecuted->registerAsFinishedJob(dynamic_cast<HPCSimulator *>(HPCsimulator));
    jobBeingExecuted = nullptr;
    addFreeNodeToScheduler(HPCsimulator);
}

void Node::addFreeNodeToScheduler(AbstractSimulator * simulator) {
    scheduler->addFreeNode(simulator, this);
}
void ReservedForSmallJobNode::addFreeNodeToScheduler(AbstractSimulator *simulator) {
    scheduler->addFreeSmallNode(simulator,this);
}

void ReservedForMediumJobNode::addFreeNodeToScheduler(AbstractSimulator * simulator) {
    scheduler->addFreeMediumNode(simulator, this);
}

void GpuNode::addFreeNodeToScheduler(AbstractSimulator *simulator) {
    scheduler->addFreeGpuNode(simulator,this);
}

bool Node::isAvailable() {
    return (jobBeingExecuted == 0);
}


/**
* Start a customer's service.  The simulator must be passed in
* as a parameter so that this can  schedule the time
* when this server will be done with the customer.
*/
void Node::insert(AbstractSimulator *simulator, AbstractJob *job) {
    if (jobBeingExecuted != nullptr) {
        /* Should never reach here */
        std::cout << "Error: I am busy serving someone else" << "\n";
    }
    jobBeingExecuted = job;
    // service time is set at average 15 patients per hour
    time = simulator->now() + job->getExecutionDuration() ;//Random::exponential(15)
    simulator->insert(this);
}

void Node::printMessage() {
    std::cout << "Finished executing " << jobBeingExecuted->getId() << " (" <<jobBeingExecuted->getType()<<") at time " << convertTime(time) << "\n";
    std::cout << "Execution duration was " << convertTime(jobBeingExecuted->getExecutionDuration()) << "\n";
    std::cout << "Job waiting time " << convertTime(time - jobBeingExecuted->getSubmittingTime()) << "\n";
    std::cout << "Job waiting time in queue "
              << convertTime(time - jobBeingExecuted->getExecutionDuration() - jobBeingExecuted->getSubmittingTime())
              << "\n";
}
