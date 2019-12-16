#ifndef _QUEUE
#define _QUEUE

#include <limits>
#include "AbstractScheduler.h"
#include "HPCParameters.h"
#include "HPCSimulator.h"

//required due to cyclic imports.
class User;

class HPCSimulator;

class AbstractScheduler;

/**
 * This class define the common API for the different categories of Jobs
 */
class AbstractJob {
protected:
    /**
     * Job id is a unique integer that
     */
    int id;   // job id
    /**
     * Static variable used to ensure that every Id is unique.
     * It is incremented at each job creation
     */
    static int jobCounter;
    /**
     * Time at which the job is submitted to the scheduler
     */
    double submittingTime = 0; // job scheduling time
    /**
     * Time at which the job finishes to run
     */
    double completionTime = 0;    // job waiting time
    /**
     * Duration of the execution of the job
     */
    double executionDuration = 0;  // job execution time
    /**
     * Number of nodes required for the job
     */
    int numberOfNodes = 0;
    /**
     * Pointer to the user who generated the job
     */
    User *user = nullptr;

    /**
     * This function is used to generate a random time between minTime and maxTime
     * It is used for generating random execution time requirements for the job.
     * The values are generated according to a normal law.
     * @param minTime the minimal execution time for this job
     * @param maxTime the maximal execution time for this job
     */
    void generateRandomTime(double minTime, double maxTime);


public:
    /**
     * The default constructor is incrementing the static job counter and setting the job id
     */
    AbstractJob();

    /**
     * To allow class inheriting from this one to define their own destructor if needed
     */
    virtual ~AbstractJob() {};

    /**
     * Return the job id
     * @return job id
     */
    int getId() const { return id; }

    /**
     * Return the time at which the job as been submitted
     * @return submitting time
     */
    double getSubmittingTime() const { return submittingTime; }

    /**
     * Return the time at which a job as been completed
     * @return completion time
     */
    double getCompletionTime() const { return completionTime; }

    /**
     * Return duration time of the job, ie. for how long it will run
     * @return execution duration
     */
    double getExecutionDuration() const { return executionDuration; }

    /**
     * Return the number of nodes on which the job is going to run
     * @return number of nodes required for this job
     */
    int getNumberOfNodes() { return (numberOfNodes); };

    /**
     * Set the time at which the job as been submitted
     * @param time of submission
     * @return this job
     */
    AbstractJob &setSubmittingTime(double time) {
        submittingTime = time;
        return *this;
    }

    /**
     * Set the time at which the job as been completed
     * @param time at which the job as been completed
     * @return this job
     */
    AbstractJob &setCompletionTime(double time) {
        completionTime = time;
        return *this;
    }

    /**
     * Set the time this job need to finish executing
     * @param time needed for this job to complete
     * @return this job
     */
    AbstractJob &setExecutionDuration(double time) {
        executionDuration = time;
        return *this;
    }

    /**
     * Set the number of nodes that this job requires
     * @param nbNodes requires
     * @return this job
     */
    AbstractJob &setNumberOfNodes(int nbNodes) {
        numberOfNodes = nbNodes;
        return *this;
    };

    /**
     * Return the user who generated this job
     * @return user who generated this job
     */
    User *getUser() const {
        return user;
    }

    /**
     * Set the user who generated this job
     * @param user who has generated this job
     */
    AbstractJob &setUser(User *user) {
        AbstractJob::user = user;
        return *this;
    }

    /**
     * Insert the job in the appropriate queue of the scheduler
     * @param simulator handling the current simulation
     * @param scheduler to which the job is submitted
     */
    virtual void insertIn(AbstractSimulator *simulator, AbstractScheduler *scheduler) = 0;

    /**
     * Return the maximum of nodes that a job of this class can use
     * @return maximum number of node
     */
    virtual int maxNodes() = 0;

    /**
     * Return the maximum time that a job of this class can use to complete
     * @return maximum duration
     */
    virtual double maxTime() = 0;

    /**
     * Return a string corresponding to the type of job
     * @return
     */
    virtual string getType() = 0;

    /**
     * Try to execute the current job, assuming it is the next in his queue.
     * Checks for nodes available according to priority rules and reservation
     * @param simulator
     * @param scheduler
     */
    virtual void tryToExecute(AbstractSimulator *simulator, AbstractScheduler *scheduler) = 0;

    /**
     * Define a comparison operation on jobs according to the highest value returned by priority
     * @return job with the highest priority
     */
    friend bool operator<(const AbstractJob &l, const AbstractJob &r) {
        return l.priority()
               > r.priority(); // keep the same order
    }

    /**
     * Return the priority of job. This function can be modified if you want to change how the
     * priority is computed. As we implemented, the older the job is the one with the highest priority
     * @return
     */
    double priority() const { return (-getSubmittingTime()); };

    /**
     * This function shall generates random requirements according to the requirements of the subclasse
     */
    virtual void generateRandomRequirements() = 0;

    /**
     * This function return true if the Job requires GPU, false in the other cases
     * @return does the job requires GPU
     */
    virtual bool isGpuJob() { return false; };

    /**
     * This function shall register this job to the appropriate category of finished jobs
     * @param pSimulator
     */
    virtual void registerAsFinishedJob(HPCSimulator *pSimulator) = 0;
};

/**
 * This class describes the behavior of large jobs
 */
class LargeJob : public AbstractJob {
private:
    string type = "Large";
public:
    void insertIn(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    int maxNodes() { return HPCParameters::largeMaxNumberOfNode; };

    double maxTime() { return HPCParameters::largeMaximumTime; };

    void tryToExecute(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    string getType() { return type; };

    void generateRandomRequirements();

    void registerAsFinishedJob(HPCSimulator *pSimulator);
};

/**
 * This class describes the behavior of medium jobs
 */
class MediumJob : public AbstractJob {
private:
    string type = "Medium";
public:

    void insertIn(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    int maxNodes() { return HPCParameters::mediumMaxNumberOfNode; };

    double maxTime() { return HPCParameters::mediumMaximumTime; };

    string getType() { return type; };

    /*
     * Assume that the job that you are trying to execute is the first in his queue
     */
    void tryToExecute(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    void generateRandomRequirements();

    void registerAsFinishedJob(HPCSimulator *pSimulator);
};

/**
 * This class describes the behavior of small jobs
 */
class SmallJob : public AbstractJob {
private:
    string type = "Small";
public:

    void insertIn(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    int maxNodes() { return HPCParameters::smallMaxNumberOfNode; };

    double maxTime() { return HPCParameters::smallMaximumTime; };

    /*
     * Assume that the job that you are trying to execute is the first in his queue
     */
    void tryToExecute(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    string getType() { return type; };

    void generateRandomRequirements();

    void registerAsFinishedJob(HPCSimulator *pSimulator);

};

/**
 * This class describes the behavior of huge jobs
 */
class HugeJob : public AbstractJob {
private:
    string type = "Huge";
public:
    void insertIn(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    int maxNodes() { return HPCParameters::hugeMaxNumberOfNode; };

    double maxTime() { return HPCParameters::hugeMaximumTime; };

    string getType() { return type; };

    void tryToExecute(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    void generateRandomRequirements();

    void registerAsFinishedJob(HPCSimulator *pSimulator);
};

/**
 * This class describes the behavior of gpu jobs
 */
class GpuJob : public AbstractJob {
private:
    string type = "Gpu";
public:

    void insertIn(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    int maxNodes() { return HPCParameters::gpuMaxNumberOfNode; };

    double maxTime() { return HPCParameters::gpuMaximumTime; };

    /*
     * Assume that the job that you are trying to execute is the first in his queue
     */
    void tryToExecute(AbstractSimulator *simulator, AbstractScheduler *scheduler);

    string getType() { return type; };

    void generateRandomRequirements();

    bool isGpuJob() override { return true; };

    void registerAsFinishedJob(HPCSimulator *pSimulator);
};


typedef AbstractJob *(*CreateJobFn)();

/**
 * This function is creating a job of a random type according to the users permissions passed
 * as a parameters and the relative proportions of the different types of jobs defined in HPC Parameters
 * @param permissions of the users (one boolean for each type of jobs: small, medium, large, huge, gpu)
 * @return a job of a random type
 */
AbstractJob *CreateRandomJob(const bool permissions[5]);

#endif
