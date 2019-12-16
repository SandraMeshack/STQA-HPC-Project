#pragma once

#include "Simulator.h"
#include "random.h"
#include "HPCSimulator.h"
#include <list>
#include <cmath>

class ListQueue;
class AbstractScheduler;
class User;
class AbstractJob;
class HPCSimulator;
/**
* A server that holds a customer for an exponentially distributed amout of time
* and releases it.
*/
class Node : public Event {
protected:
    /**
     * Job currently being executed by the node
     */
	AbstractJob* jobBeingExecuted;
    /**
     * scheduler (for saying that the node is free)
     */
    AbstractScheduler* scheduler;

public:
    /**
     * Set jobBeing executed to nullptr
     */
	Node();
	Node(const Node& node) = delete;
	Node& operator=(const Node& node) = delete;

	/**
	 * Set the scheduler for this node
	 * @param scheduler
	 * @return this Node
	 */
	Node& addScheduler(AbstractScheduler* scheduler);
	/**
	 *  A node event execute when the job it is executing is done.
	 *  Therefore it reduce the number of nodes currently use of the job's user by one
	 *  It sets the completion time of the job
	 *  It registers the job as finished
	 *  set jobBeingExecuted to null pointer and tell the scheduler that he is free to execute a new job
	 * @param simulator
	 */
	void execute(AbstractSimulator *simulator);

	/**
	 * Return true if the node is not executing a job
	 * @return
	 */
	bool isAvailable();

    /**
     * Set the nex job to execute
     * @param simulator
     * @param job
     */
	void insert(AbstractSimulator* simulator, AbstractJob* job);

	/**
	 * Print an end of execution message
	 */
	void printMessage();

	/**
	 * Tells the scheduler that this normal node is free for getting a new job
	 * @param Simulator
	 */
	virtual void addFreeNodeToScheduler(AbstractSimulator *Simulator);
};

/**
 * This class is for the nodes that are reserved for medium jobs
 */
class ReservedForMediumJobNode : public Node {
public:
    /**
     * Tells the scheduler that this node is free for getting a new  medium job
     * @param Simulator
     */
    void addFreeNodeToScheduler(AbstractSimulator *simulator) override;
};


/**
 * This class is for the nodes that are reserved for small jobs
 */
class ReservedForSmallJobNode : public Node {
public:
    /**
    * Tells the scheduler that this node is free for getting a new small job
    * @param Simulator
    */
void addFreeNodeToScheduler(AbstractSimulator *simulator) override;
};


/**
 * This class is for the nodes equiped with GPU
 */
class GpuNode : public Node {
public:
    /**
    * Tells the scheduler that this node equipped with gpu is free for getting a new small job
    * @param Simulator
    */

void addFreeNodeToScheduler(AbstractSimulator* Simulator) override;
};
