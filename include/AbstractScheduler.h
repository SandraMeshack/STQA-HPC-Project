#ifndef SUPERCOMPUTERSIMULATION_ABSTRACTSCHEDULER_H
#define SUPERCOMPUTERSIMULATION_ABSTRACTSCHEDULER_H

#include <queue>
#include "../include/Node.h"
#include "AbstractJob.h"
#include <list>

//required due to cyclic includes
class User;

class MediumJob;

class SmallJob;

class LargeJob;

class HugeJob;

class GpuJob;

class Node;

class ReservedForMediumJobNode;

class ReservedForSmallJobNode;

class GpuNode;

/**
 * This class describes the API for a scheduler. This is the class that you need to inherit from in order
 * to be able to use your new scheduler in the simulation.
 */
class AbstractScheduler {
protected:
    /**
     * queue for medium jobs
     */
    std::list<MediumJob *> *mediumJobs{};
    /**
     * queue for small jobs
     */
    std::list<SmallJob *> *smallJobs{};

    /**
     * queue for large jobs
     */
    std::list<LargeJob *> *largeJobs{};

    /**
     * queue for huge jobs
     */
    std::list<HugeJob *> *hugeJobs{};

    /**
     * queue for gpu jobs
     */
    std::list<GpuJob *> *gpuJobs{};

    /**
     * list of free nodes reserved for medium jobs
     */
    std::queue<Node *> freeMediumNodes;
    /**
     * list of free nodes reserved for small jobs
     */
    std::queue<Node *> freeSmallNodes;

    /**
     * list of free nodes equiped with gpu
     */
    std::queue<Node *> freeGpuNodes;

    /**
     * list of free normal nodes
     */
    std::queue<Node *> freeNodes;

public:

    /**
     * Try to "execute" the next job in the large queue according to priority rules
     */
    virtual void tryToExecuteNextLargeJob(AbstractSimulator *simulator) = 0;

    /**
     * Try to "execute" the next job in the medium queue according to priority rules
    */

    virtual void tryToExecuteNextMediumJob(AbstractSimulator *simulator) = 0;

    /**
     * Try to "execute" the next job in the small queue according to priority rules
    */
    virtual void tryToExecuteNextSmallJob(AbstractSimulator *simulator) = 0;

    /**
    * Try to execute as much huge jobs as possible across all available nodes
    * This function should only be called at the beginning of the week-end
    */
    virtual void tryToExecuteNextHugeJobs(AbstractSimulator *simulator) = 0;

    /**
    * Try to "execute" the next job in the gpu queue according to priority rules
    */
    virtual void tryToExecuteNextGpuJob(AbstractSimulator *simulator) = 0;

    /**
     * Try to execute the next small enough job. This is checking for priority
     * and week-end cut-off
     * @param simulator
     */
    virtual void tryToExecuteNextNonGpuJobShortEnough(AbstractSimulator *simulator) = 0;

    /**
     * Creating a the required instances for the queues
     */
    AbstractScheduler();

    //Removing copy constructor
    AbstractScheduler(const AbstractScheduler &scheduler) = delete;

    //Removing = operator
    AbstractScheduler &operator=(const AbstractScheduler &scheduler) = delete;

    /**
    * Deleting a the required instances for the queues
    */
    virtual ~AbstractScheduler();

    /**
     * add a free node to the list of free nodes kept by the scheduler
     * Try to trigger the next job without GPU requirement according to week-end cut-off
     * @param simulator running the current simulation
     * @param node to add
     */
    virtual void addFreeNode(AbstractSimulator *simulator, Node *node) = 0;

    /**
     * add a free node reserved for medium jobs to the list of free nodes kept by the scheduler
     * Try to trigger the next medium job according to week-end cut-off and priority
     * @param simulator running the current simulation
     * @param node to add
     */
    virtual void addFreeMediumNode(AbstractSimulator *simulator, ReservedForMediumJobNode *node) = 0;

    /**
     * add a free node reserved for small job to the list of free nodes kept by the scheduler
     * Try to trigger the next small job according to week-end cut-off and priority
     * @param simulator running the current simulation
     * @param node to add
     */
    virtual void addFreeSmallNode(AbstractSimulator *simulator, ReservedForSmallJobNode *node) = 0;

    /**
     * add a free node equipped with Gpu to the list of free nodes kept by the scheduler
     * Try to trigger the next job including gpu jobs according to week-end cut-off and priority
     * @param simulator running the current simulation
     * @param node to add
     */
    virtual void addFreeGpuNode(AbstractSimulator *simulator, GpuNode *node) = 0;

    /**
     * insert a medium job in the corresponding queue.
     * Try to run it according to priority if the queue is empty
     * @param simulator
     * @param job
     */
    virtual void insertMediumJob(AbstractSimulator *simulator, MediumJob *job) = 0;

    /**
    * insert a small job in the corresponding queue.
    * Try to run it according to priority if the queue is empty
    * @param simulator
    * @param job
    */
    virtual void insertSmallJob(AbstractSimulator *simulator, SmallJob *job) = 0;

    /**
     * insert a large job in the corresponding queue.
     * Try to run it according to priority if the queue is empty
     * @param simulator
     * @param job
     */
    virtual void insertLargeJob(AbstractSimulator *simulator, LargeJob *job) = 0;

    /**
    * insert a gpu job in the corresponding queue.
    * Try to run it according to priority if the queue is empty
    * @param simulator
    * @param job
    */
    virtual void insertGpuJob(AbstractSimulator *simulator, GpuJob *job) = 0;

    /**
    * insert a Huge job in the corresponding queue.
    * Huge jobs should only start at the beginning of the week-end to be sure that
    * they have enough time to run. Therefore it does not try to run the next huge
    * job even if the queue is empty and we are during the week-end.
    * @param simulator
    * @param job
    */
    virtual void insertHugeJob(AbstractSimulator *simulator, HugeJob *job) = 0;

    /**
     * try to execute as much non Huge jobs as possible according to priority
     * This ran on monday morning at the end of the week-end cut-off.
     * This does not check for week-end cut-off
     * @param pSimulator
     */
    virtual void tryToExecuteNextJobs(AbstractSimulator *pSimulator) = 0;

    /**
     * Return the total of non-huge job waiting in the different queues
     * @return
     */
    int totalOfNonHugeJobsWaiting();
};


/**
 * This class derived from Abstract Scheduler and is a scheduler working on the
 * first in first out principle.
 */
class Scheduler : public AbstractScheduler {

public:

    Scheduler();

    void tryToExecuteNextLargeJob(AbstractSimulator *simulator);

    void tryToExecuteNextMediumJob(AbstractSimulator *simulator);

    void tryToExecuteNextSmallJob(AbstractSimulator *simulator);

    void tryToExecuteNextHugeJobs(AbstractSimulator *simulator);

    void tryToExecuteNextGpuJob(AbstractSimulator *simulator);

    void tryToExecuteNextNonGpuJobShortEnough(AbstractSimulator *simulator);

    //removing the copy constructor
    Scheduler(const Scheduler &scheduler) = delete;

    //removing the  equal operator
    Scheduler &operator=(const Scheduler &scheduler) = delete;

    void addFreeNode(AbstractSimulator *simulator, Node *node);

    void addFreeMediumNode(AbstractSimulator *simulator, ReservedForMediumJobNode *node);

    void addFreeSmallNode(AbstractSimulator *simulator, ReservedForSmallJobNode *node);

    void addFreeGpuNode(AbstractSimulator *simulator, GpuNode *node);

    /**
     * Return the next non-gpu Job
     * @return
     */
    AbstractJob *nextNonGpuJob();

    /**
     * Return the next job including gpu jobs
     * @return
     */
    AbstractJob *nextJob();

    void insertMediumJob(AbstractSimulator *simulator, MediumJob *job);

    void insertSmallJob(AbstractSimulator *simulator, SmallJob *job);

    void insertLargeJob(AbstractSimulator *simulator, LargeJob *job);

    void insertGpuJob(AbstractSimulator *simulator, GpuJob *job);

    void insertHugeJob(AbstractSimulator *simulator, HugeJob *job);

    void tryToExecuteNextJobs(AbstractSimulator *pSimulator);

};
/**
 * This function return if the time passed as argument is during we are during the week-end
 * rely on the assumption that the simulation starts at 9AM on Monday.
 * @param time to assess if it is in the wek-end or not
 * @return
 */
bool isDuringWeekend(double time);

/**
 * This function allows us to compare two job pointers according to their submitting time.
 * The one with the lowest submission time is (for the FIFO scheduler) the one with the highest
 * priority. Therefore this is the one which is seen as the maximum in the comparison.
 * @param i an abstract job pointer
 * @param j an abstract job pointer
 * @return the abstract job pointer with the lowest submission time
 */
bool comparingJobsPointersSubmittingTime(AbstractJob *i, AbstractJob *j);

#endif //SUPERCOMPUTERSIMULATION_ABSTRACTSCHEDULER_H
