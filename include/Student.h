//
// Created by inao on 12/12/2019.
//

#ifndef SUPERCOMPUTERSIMULATION_STUDENT_H
#define SUPERCOMPUTERSIMULATION_STUDENT_H


#include "User.h"
#include "Curriculum.h"

/**
 * This class defines the behavior of students
 */
class Student : public User {
private:
    /**
     * curriculum in which the student is enrolled in
     */
    Curriculum *curriculum;
public:
    Student() = delete;

    /**
     * The student is using the Curriculum to initialise its instantaneous cap and individual budget
     * @param curriculum
     */
    Student(Curriculum *curriculum);


    /**
     * The student is using the Curriculum to initialise its instantaneous cap and individual budget
     * @param curriculum
     */
    Student(Curriculum *curriculum, double meanTimeBetweenTwoJobs);


    /**
     * The student is using the Curriculum to initialise its instantaneous cap and individual budget
     * @param curriculum
     */
    Student(Curriculum *curriculum, double meanTimeBetweenTwoJobs, double firstJobTime);

    Student(const Student &g) = delete;

    Student &operator=(const Student &g) = delete;

};


#endif //SUPERCOMPUTERSIMULATION_STUDENT_H
