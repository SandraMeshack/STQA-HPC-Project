#pragma once

#include<vector>
#include <set>
#include "Simulator.h"
#include "Group.h"
#include "Curriculum.h"
#include "User.h"

class GpuJob;

class SmallJob;

class MediumJob;

class LargeJob;

class HugeJob;

class User;

class HPCSimulator : public Simulator {
private:
    /**
     * Holding the list of the users generated from the input file
     */
    std::vector<User *> users;
    /**
     * Register all the finished gpu jobs
     */
    std::unordered_set<GpuJob *> finishedGpuJobs;
    /**
     * Register all the finished small jobs
     */
    std::unordered_set<SmallJob *> finishedSmallJobs;
    /**
    * Register all the finished medium jobs
    */
    std::unordered_set<MediumJob *> finishedMediumJobs;
    /**
    * Register all the finished large jobs
    */
    std::unordered_set<LargeJob *> finishedLargeJobs;
    /**
     * Register all the finished huge jobs
     */
    std::unordered_set<HugeJob *> finishedHugeJobs;
    /**
    * Register how much budget can be spent by the users generated from the input file
    */
    double initialUsersBudget = 0;

public:
    HPCSimulator() = default;

    /**
     * Parse the file for generating Students, Researches, Groups and Curriculum
     * @param filename
     */
    void initialisation(string filename);

    /**
     * Create the scheduler, initialise the Nodes according to HPCParameters.
     * initialise every elements required for the simulation
     * launch the simulation and cleans up
     */
    void start();

    /**
     * Register a finished Gpu Job
     * @param pJob
     */
    void registerFinishedGpuJobs(GpuJob *pJob);


    /**
     * Register a finished Small Job
     * @param pJob
     */
    void registerFinishedSmallJobs(SmallJob *pJob);


    /**
     * Register a finished Medium Job
     * @param pJob
     */
    void registerFinishedMediumJobs(MediumJob *pJob);

    /**
     * Register a finished Large Job
     * @param pJob
     */
    void registerFinishedLargeJobs(LargeJob *pJob);

    /**
     * Register a finished Huge Job
     * @param pJob
     */
    void registerFinishedHugeJobs(HugeJob *pJob);

    /**
     * Compute measurements for the simulation and print the results
     */
    void printResults();
};


