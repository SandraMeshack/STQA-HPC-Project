
#ifndef SUPERCOMPUTERSIMULATION_RESEARCHER_H
#define SUPERCOMPUTERSIMULATION_RESEARCHER_H


#include "User.h"
#include "Group.h"

/**
 * This class defines the behavior of researchers
 */
class Researcher : public User {
private:
    /**
     * Research team to which the researcher is affiliated
     */
    Group *group;
public:
    Researcher() = delete;

    Researcher(Group *group);

    Researcher(Group *group, double meanTimeBetweenTwoJobs);

    Researcher(Group *group, double meanTimeBetweenTwoJobs, double firstJobTime);

    Researcher(const Researcher &g) = delete;

    Researcher &operator=(const Researcher &g) = delete;

/**
 * Add an individual grant to the budget of a researcher
 * @param grantedBudget
 */
    void addIndividualGrant(double grantedBudget) { budget += grantedBudget; };

    /**
     * Return the budget left for a researcher taking into acount its individual
     * grant and the group resources
     * @return
     */
    double budgetLeft() override;

    /**
     * Remove a certain amout from the budget.
     * Starts by removing from the group resources, then on its individual budget
     * @param amountToRemove
     */
    void removeFromBudget(double amountToRemove) override;

};


#endif //SUPERCOMPUTERSIMULATION_RESEARCHER_H
