#include "../include/Student.h"

Student::Student(Curriculum *curriculum) : curriculum(curriculum), User() {
    budget = curriculum->getCumulativeCapInNodeHour();
    instantaneousMaxNumberOfNodes = curriculum->getInstantaneousCapInNode();
}

Student::Student(Curriculum *curriculum, double meanTimeBetweenTwoJobs) : User(meanTimeBetweenTwoJobs),
                                                                          curriculum(curriculum) {
    budget = curriculum->getCumulativeCapInNodeHour();
    instantaneousMaxNumberOfNodes = curriculum->getInstantaneousCapInNode();
}

Student::Student(Curriculum *curriculum, double meanTimeBetweenTwoJobs, double firstJobTime) : User(
        meanTimeBetweenTwoJobs, firstJobTime), curriculum(curriculum) {
    budget = curriculum->getCumulativeCapInNodeHour();
    instantaneousMaxNumberOfNodes = curriculum->getInstantaneousCapInNode();
}
