//
// Created by inao on 13/12/2019.
//

#ifndef SUPERCOMPUTERSIMULATION_HPCPARAMETERS_H
#define SUPERCOMPUTERSIMULATION_HPCPARAMETERS_H
#include <vector>

/**
 * This is a static class designed to hold the parameters of HPC machine
 * It includes maximum number of nodes and duration for several job categories,
 * number of nodes, number of accelerated nodes and operation cost and prices.
 *
 */
class HPCParameters {
public:
    /**
     * Total Number of Computing Nodes on the HPC Plateform
     * It is assume that this number is coherent with the numbers of specific nodes
     * We reserve 10 % of those nodes to small jobs, 30% to medium ones.
     * ASSUMPTION : every nodes correspond to 16 cores
     */
    static const int TotalNumberOfNodes = 128;
    /**
     * Number of Nodes accelerated with to GPU dedicated to computations
     * Those are counted as part of the total number of Nodes
     */
    static const int NumberOfGpuNodes = 8;

    /**
     * Maximum number of Nodes for a small job
     */
    const static int smallMaxNumberOfNode = 1;
    /**
     * Maximum amount of time a small job shall take to complete
     */
    constexpr static double smallMaximumTime = 2;

    /**
     * Maximum number of Nodes for a medium job
     */
    const static int mediumMaxNumberOfNode = 0.1 * TotalNumberOfNodes;
    /**
     * Maximum amount of time a medium job shall take to complete
     */
    constexpr static double mediumMaximumTime = 8;

    /**
     *Maximum number of Nodes for a large job
     */
    const static int largeMaxNumberOfNode = 0.5 * TotalNumberOfNodes;
    /**
     * Maximum amount of time a latge job shall take to complete
     */
    constexpr static double largeMaximumTime = 16;

    /**
     * Maximum number of Nodes for a huge job
     */
    const static int hugeMaxNumberOfNode = TotalNumberOfNodes;
    /**
     * Maximum amount of time a huge job shall take to complete
     */
    constexpr static double hugeMaximumTime = 64;

    /**
    * Maximum number of Nodes for a gpu job
    */
    constexpr const static double gpuMaximumTime = largeMaximumTime;
    /**
     * Maximum amount of time a gpu job shall take to complete
     */
    const static int gpuMaxNumberOfNode = NumberOfGpuNodes;

    /**
     * This vector holds relative proportions of the different types of jobs : small, medium, large, huge, gpu
     */
    const static std::vector<int> jobTypeProportions;

    /**
     * Price for using one normal node during one hour
     */
    constexpr double const static costOneHourOneNode = 1;
    /**
     * Price for using a gpu node during one hour
     */
    constexpr double const static costOneHourOneGPUNode = 1.1;
    /**
     * Cost for operating one Node during one hour
     */
    constexpr double const static operationCostOneHourOneNode = 0.1;
    /**
     * Cost for running the HPC infrastructure during oneHour
     */
    constexpr double const static overallOperationCostPerHour = operationCostOneHourOneNode * TotalNumberOfNodes;
};

#endif //SUPERCOMPUTERSIMULATION_HPCPARAMETERS_H
