#ifndef SUPERCOMPUTERSIMULATION_CURRICULUM_H
#define SUPERCOMPUTERSIMULATION_CURRICULUM_H

/**
 * This class defines a curriculum
 * It is mainly defined by to caps set at the instantiation and getters for those caps.
 */
class Curriculum {
    /**
     * total budget that a student of the curriculum can use
     */
    const double cumulativeCapPerStudentInNodeHour;
    /**
     * maximum number of Nodes that the student can ask for the same time
     */
    const int instantaneousCapPerStudentInNodes;

public:
    /**
     * Return the maximum number of Nodes that the student can ask for the same time
     * @return maximum number of Nodes that the student can ask for the same time
     */
    int getInstantaneousCapInNode() { return instantaneousCapPerStudentInNodes; };

    /**
     * Return the total budget that a student of the curriculum can use
     * @return total budget that a student of the curriculum can use
     */
    double getCumulativeCapInNodeHour() { return cumulativeCapPerStudentInNodeHour;};

    /**
     * This generates a new curriculum using the following parameters
     * @param cumulativeCap : total budget that a student of the curriculum can use
     * @param instantaneousCap :maximum number of Nodes that the student can ask for the same time
     */
    Curriculum(double cumulativeCap, int instantaneousCap) : cumulativeCapPerStudentInNodeHour(cumulativeCap),
                                                             instantaneousCapPerStudentInNodes(instantaneousCap) {};
    Curriculum()=delete;
};


#endif //SUPERCOMPUTERSIMULATION_CURRICULUM_H
