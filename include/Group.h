#ifndef SUPERCOMPUTERSIMULATION_GROUP_H
#define SUPERCOMPUTERSIMULATION_GROUP_H

/**
 * This class defines a group, to understand as a researcher team
 * The team has a common pool of resources that can be used by any member of the group
 */
class Group {
    /**
     * Common pool of resources hold by the group
     */
    double groupBudget;

public:

    /**
     * Return the current budget left for the group
     * @return
     */
    double getGroupBudget() { return groupBudget; };

    /**
     * Remove a certain amount from the group budget
     * @param amountToRemove
     */
    void removeFromGroupRessources(
            double amountToRemove) { groupBudget -= amountToRemove; };


    /**
     * Constructor creating a group with a initial budget fixed by the parameter
     * @param commonBudget
     */
    Group(double commonBudget) : groupBudget(commonBudget) {};

    Group() = delete;
};

#endif //SUPERCOMPUTERSIMULATION_GROUP_H
