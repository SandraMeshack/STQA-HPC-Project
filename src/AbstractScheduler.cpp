#include <algorithm>
#include "../include/AbstractScheduler.h"
#include "../include/User.h"


bool comparingJobsPointersSubmittingTime(AbstractJob *i, AbstractJob *j) { return j->getSubmittingTime() < i->getSubmittingTime(); }

/*
 * Assuming that the simulation start at Monday 9 AM.
 */
bool isDuringWeekend(double time) {
    const double numberOfHourInTheWeek = 168;
    const double startingWeekendTime = 104;
    double mod = fmod(time, numberOfHourInTheWeek);
    return (mod > startingWeekendTime);
}


AbstractScheduler::AbstractScheduler() {
    mediumJobs = new std::list<MediumJob *>;
    smallJobs = new std::list<SmallJob *>;
    largeJobs = new std::list<LargeJob *>;
    hugeJobs = new std::list<HugeJob *>;
    gpuJobs = new std::list<GpuJob *>;

}

AbstractScheduler::~AbstractScheduler() {
    delete mediumJobs;
    delete largeJobs;
    delete hugeJobs;
    delete smallJobs;
    delete gpuJobs;

}

void Scheduler::addFreeMediumNode(AbstractSimulator *simulator, ReservedForMediumJobNode *node) {
    this->freeMediumNodes.push(node);
    //if we are during the week and there is enough time for a medium job we try to tun the next one
    if (!isDuringWeekend(simulator->now()) && !isDuringWeekend(simulator->now() + HPCParameters::mediumMaximumTime)) {
        tryToExecuteNextMediumJob(simulator);
    }
}

void Scheduler::addFreeSmallNode(AbstractSimulator *simulator, ReservedForSmallJobNode *node) {
    this->freeSmallNodes.push(node);
    //if we are during the week and there is enough time for a small job we try to tun the next one
    if (!isDuringWeekend(simulator->now()) && !isDuringWeekend(simulator->now() + HPCParameters::smallMaximumTime)) {
        tryToExecuteNextSmallJob(simulator);
    }
}

void Scheduler::addFreeNode(class AbstractSimulator *simulator, class Node *node) {
    this->freeNodes.push(node);
    tryToExecuteNextNonGpuJobShortEnough(simulator);

}

void Scheduler::addFreeGpuNode(AbstractSimulator *simulator, GpuNode *node) {
    this->freeGpuNodes.push(node);
    if (!gpuJobs->empty() && gpuJobs->front() == nextJob()) {
        if (!isDuringWeekend(simulator->now()) && !isDuringWeekend(simulator->now() + HPCParameters::gpuMaximumTime)) {
            tryToExecuteNextGpuJob(simulator);
        } else {
            tryToExecuteNextNonGpuJobShortEnough(simulator);
        }
    } else {
        tryToExecuteNextNonGpuJobShortEnough(simulator);
    }
}


void Scheduler::insertMediumJob(AbstractSimulator *simulator, MediumJob *job) {
    if (!isDuringWeekend(simulator->now()) && mediumJobs->empty() &&
        !isDuringWeekend(simulator->now() + HPCParameters::mediumMaximumTime)) {
        mediumJobs->push_back(job);
        tryToExecuteNextMediumJob(simulator);
    } else {
        mediumJobs->push_back(job);
    }
}

void Scheduler::insertSmallJob(AbstractSimulator *simulator, SmallJob *job) {
    if (!isDuringWeekend(simulator->now()) && smallJobs->empty() &&
        !isDuringWeekend(simulator->now() + HPCParameters::smallMaximumTime)) {
        smallJobs->push_back(job);
        tryToExecuteNextSmallJob(simulator);
    } else {
        smallJobs->push_back(job);
    }
}

void Scheduler::insertGpuJob(AbstractSimulator *simulator, GpuJob *job) {
    if (!isDuringWeekend(simulator->now()) && gpuJobs->empty() &&
        !isDuringWeekend(simulator->now() + HPCParameters::gpuMaximumTime)) {
        gpuJobs->push_back(job);
        tryToExecuteNextGpuJob(simulator);
    } else {
        gpuJobs->push_back(job);
    }
}


void Scheduler::insertLargeJob(class AbstractSimulator *simulator, class LargeJob *job) {
    if (!isDuringWeekend(simulator->now()) && largeJobs->empty() &&
        !isDuringWeekend(simulator->now() + HPCParameters::largeMaximumTime)) {
        largeJobs->push_back(job);
        tryToExecuteNextLargeJob(simulator);
    } else {
        largeJobs->push_back(job);
    }
}

void Scheduler::insertHugeJob(AbstractSimulator *simulator, HugeJob *job) {
    hugeJobs->push_back(job);
}

//TODO TEST THAT
AbstractJob *Scheduler::nextNonGpuJob() {
    std::vector<AbstractJob *> nextJobs;

    MediumJob *nextMediumJob;
    if (!mediumJobs->empty()) {
        nextJobs.push_back(mediumJobs->front());
    }
    if (!largeJobs->empty()) {
        nextJobs.push_back(largeJobs->front());
    }

    if (!smallJobs->empty()) {
        nextJobs.push_back(smallJobs->front());
    }

    if (nextJobs.empty()) {
        return nullptr;
    }
    // TO-DO :especially this
    auto nextJobIt = max_element(std::begin(nextJobs), std::end(nextJobs), comparingJobsPointersSubmittingTime);
    return *nextJobIt;
}

//TODO TEST ALSO THAT
AbstractJob *Scheduler::nextJob() {
    AbstractJob *nextNotGpuJob = nextNonGpuJob();
    if (!gpuJobs->empty()) {
        AbstractJob *nextGpuJob = gpuJobs->front();
        if (nextNotGpuJob == nullptr) {
            return nextGpuJob;
        } else {
            return max(nextGpuJob, nextNotGpuJob, comparingJobsPointersSubmittingTime);
        }
    }
    return nextNonGpuJob();
}

/*
 * According to priorities
 */
void Scheduler::tryToExecuteNextNonGpuJobShortEnough(AbstractSimulator *simulator) {
    AbstractJob *job;
    if (!isDuringWeekend(simulator->now()) && !isDuringWeekend(simulator->now() + HPCParameters::largeMaximumTime)) {
        job = nextNonGpuJob();
        if (job != nullptr) {
            job->tryToExecute(simulator, this);
            return;
        }
    } else if (!isDuringWeekend(simulator->now()) &&
               !isDuringWeekend(simulator->now() + HPCParameters::mediumMaximumTime)) {
        if (mediumJobs->empty() || (!smallJobs->empty() && *(mediumJobs->front()) < *(smallJobs->front()))) {
            tryToExecuteNextSmallJob(simulator);
            return;
        }
        tryToExecuteNextMediumJob(simulator);
        return;
    } else if (!isDuringWeekend(simulator->now()) && !isDuringWeekend(simulator->now() + HPCParameters::smallMaximumTime)) {
        tryToExecuteNextSmallJob(simulator);
    }
}


/*
 * Take into account priority.
 *
 */
void Scheduler::tryToExecuteNextLargeJob(AbstractSimulator *simulator) {

    LargeJob *nextLargeJob;
    if (!largeJobs->empty()) {
        nextLargeJob = largeJobs->front();
        if (nextLargeJob == nextJob() &&
            (freeNodes.size() + freeGpuNodes.size()) >= nextLargeJob->getNumberOfNodes()) {
            for (int i = 0; i < nextLargeJob->getNumberOfNodes(); ++i) {
                Node *node;
                if (!freeNodes.empty()) {
                    node = freeNodes.front();
                    freeNodes.pop();
                } else {
                    node = freeGpuNodes.front();
                    freeGpuNodes.pop();
                }
                node->insert(simulator, nextLargeJob);
            }
            largeJobs->pop_front();
        } else if (nextLargeJob == nextNonGpuJob() && freeNodes.size() >= nextLargeJob->getNumberOfNodes()) {
            for (int i = 0; i < nextLargeJob->getNumberOfNodes(); ++i) {
                Node *node = freeNodes.front();
                freeNodes.pop();
                node->insert(simulator, nextLargeJob);
            }
            largeJobs->pop_front();
        }
    }
}


void Scheduler::tryToExecuteNextMediumJob(AbstractSimulator *simulator) {
    AbstractJob *nextMediumJob;
    if (!mediumJobs->empty()) {
        nextMediumJob = mediumJobs->front();
        if (nextMediumJob == nextJob() &&
            (freeMediumNodes.size() + freeNodes.size() + freeGpuNodes.size()) >=
            nextMediumJob->getNumberOfNodes()) {
            for (int i = 0; i < nextMediumJob->getNumberOfNodes(); ++i) {
                Node *node;
                if (!freeMediumNodes.empty()) {
                    node = freeMediumNodes.front();
                    freeMediumNodes.pop();

                } else if (!freeNodes.empty()) {
                    node = freeNodes.front();
                    freeNodes.pop();
                } else {
                    node = freeGpuNodes.front();
                    freeGpuNodes.pop();
                }
                node->insert(simulator, nextMediumJob);
            }
            mediumJobs->pop_front();
            return;
        } else if ((freeMediumNodes.size() + freeNodes.size()) >= nextMediumJob->getNumberOfNodes() &&
                   nextMediumJob == nextNonGpuJob()) {
            for (int i = 0; i < nextMediumJob->getNumberOfNodes(); ++i) {
                Node *node;
                if (!freeMediumNodes.empty()) {
                    node = freeMediumNodes.front();
                    freeMediumNodes.pop();

                } else {
                    node = freeNodes.front();
                    freeNodes.pop();
                }
                node->insert(simulator, nextMediumJob);
            }
            mediumJobs->pop_front();
            return;
        } else if (freeMediumNodes.size() >= nextMediumJob->getNumberOfNodes()) {
            for (int i = 0; i < nextMediumJob->getNumberOfNodes(); ++i) {
                Node *node = freeMediumNodes.front();
                freeMediumNodes.pop();
                node->insert(simulator, nextMediumJob);
            }
            mediumJobs->pop_front();
            return;
        }
    }

}

void Scheduler::tryToExecuteNextGpuJob(AbstractSimulator *simulator) {
    AbstractJob *nextGPUJob;
    if (!gpuJobs->empty()) {
        nextGPUJob = gpuJobs->front();
        if (nextGPUJob == nextJob() && freeGpuNodes.size() >= nextGPUJob->getNumberOfNodes()) {
            for (int i = 0; i < nextGPUJob->getNumberOfNodes(); ++i) {
                Node *node = freeGpuNodes.front();
                freeGpuNodes.pop();
                node->insert(simulator, nextGPUJob);
            }
            gpuJobs->pop_front();
        }
    }
}



void Scheduler::tryToExecuteNextSmallJob(AbstractSimulator *simulator) {
    AbstractJob *nextSmallJob;
    if (!smallJobs->empty()) {
        nextSmallJob = smallJobs->front();
        //if small job is the overall next job, it can use ressources from freeSmallNode, freeGpuNode, freeNode
        if (nextSmallJob == nextJob() &&
            (freeSmallNodes.size() + freeNodes.size() + freeGpuNodes.size()) >=
            nextSmallJob->getNumberOfNodes()) {
            for (int i = 0; i < nextSmallJob->getNumberOfNodes(); ++i) {
                Node *node;
                if (!freeSmallNodes.empty()) {
                    node = freeSmallNodes.front();
                    freeSmallNodes.pop();

                } else if (!freeNodes.empty()) {
                    node = freeNodes.front();
                    freeNodes.pop();
                } else {//Gpu nodes are taken as last ressources as they are the more valuable
                    node = freeGpuNodes.front();
                    freeGpuNodes.pop();
                }
                node->insert(simulator, nextSmallJob);
            }
            smallJobs->pop_front();
            return;
            //if it is the next nonGpu node it can use the resources from freeSmallNodes and freeNodes
        } else if ((freeSmallNodes.size() + freeNodes.size()) >= nextSmallJob->getNumberOfNodes() &&
                   nextSmallJob == nextNonGpuJob()) {
            for (int i = 0; i < nextSmallJob->getNumberOfNodes(); ++i) {
                Node *node;
                if (!freeSmallNodes.empty()) {
                    node = freeSmallNodes.front();
                    freeSmallNodes.pop();
                } else {
                    node = freeNodes.front();
                    freeNodes.pop();
                }
                node->insert(simulator, nextSmallJob);
            }
            smallJobs->pop_front();
            return;
            //if it is not the next non-gpu job, it can only be run on freeSmallNodes resources
        } else if (freeSmallNodes.size() >= nextSmallJob->getNumberOfNodes()) {
            for (int i = 0; i < nextSmallJob->getNumberOfNodes(); ++i) {
                Node *node = freeSmallNodes.front();
                freeSmallNodes.pop();
                node->insert(simulator, nextSmallJob);
            }
            smallJobs->pop_front();
            return;
        }
    }

}


/* Try to launch as many huge jobs as possible */
void Scheduler::tryToExecuteNextHugeJobs(AbstractSimulator *simulator) {
    int previousHugeQueueSize;
    do {
        previousHugeQueueSize = hugeJobs->size();
        if (!hugeJobs->empty()) {
            AbstractJob *nextHugeJob = hugeJobs->front();
            int totalNumberOfNodesAvailable =
                    freeNodes.size() + freeMediumNodes.size() + freeSmallNodes.size() + freeGpuNodes.size();
            if (nextHugeJob != nullptr && nextHugeJob->getNumberOfNodes() < totalNumberOfNodesAvailable) {
                for (int i = 0; i < nextHugeJob->getNumberOfNodes(); ++i) {
                    Node *node;
                    if (!freeSmallNodes.empty()) {
                        node = freeSmallNodes.front();
                        freeSmallNodes.pop();
                    } else if (!freeMediumNodes.empty()) {
                        node = freeMediumNodes.front();
                        freeMediumNodes.pop();
                    } else if (!freeNodes.empty()) {
                        node = freeNodes.front();
                        freeNodes.pop();
                    } else {
                        node = freeGpuNodes.front();
                        freeGpuNodes.pop();
                    }
                    node->insert(simulator, nextHugeJob);
                }
                hugeJobs->pop_front();
            }
        }
    } while (previousHugeQueueSize > hugeJobs->size());
}

/*
 * Do not check for the week-end
 * Normally ran only at the begining of the week
*/
void Scheduler::tryToExecuteNextJobs(AbstractSimulator *simulator) {
    int previousNumberOfJobWaiting;
    do {
        previousNumberOfJobWaiting = totalOfNonHugeJobsWaiting();
        if (nextJob() != nullptr) {
            nextJob()->tryToExecute(simulator, this);
        }
    } while (previousNumberOfJobWaiting > totalOfNonHugeJobsWaiting());
    do {
        previousNumberOfJobWaiting = totalOfNonHugeJobsWaiting();
        if (nextNonGpuJob() != nullptr) {
            nextNonGpuJob()->tryToExecute(simulator, this);
        }
    } while (previousNumberOfJobWaiting > totalOfNonHugeJobsWaiting());
}

int AbstractScheduler::totalOfNonHugeJobsWaiting() {
    return smallJobs->size() + mediumJobs->size() + largeJobs->size() + gpuJobs->size();
}

Scheduler::Scheduler() {

}