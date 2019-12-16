#include "../include/HPCSimulator.h"
#include "../include/AbstractScheduler.h"
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Researcher.h"
#include "../include/weekendEvent.h"
#include "../include/Group.h"
#include "../include/ListQueue.h"
#include <cmath>
#include <fstream>

void HPCSimulator::start() {
    AbstractScheduler *scheduler = new Scheduler();
    events = new ListQueue();

    auto *weekendBegin = new WeekendBegin(scheduler);
    auto *weekendEnd = new WeekendEnd(scheduler);
    insert(weekendBegin);
    insert(weekendEnd);

    /* Create the generator, queue, and simulator */
    /* Connect them together. */
    int numberOfNodesAdded = 0;
    std::vector<Node *> nodes;
    for (int i = 0; i < HPCParameters::NumberOfGpuNodes; ++i) {
        nodes.push_back(new GpuNode());
        numberOfNodesAdded++;
    }
    for (int i = 0; i < 0.1 * HPCParameters::TotalNumberOfNodes; ++i) {
        nodes.push_back(new ReservedForSmallJobNode());
        numberOfNodesAdded++;
    }
    for (int i = 0; i < 0.3 * HPCParameters::TotalNumberOfNodes; ++i) {
        nodes.push_back(new ReservedForMediumJobNode());
        numberOfNodesAdded++;
    }
    for (int i = numberOfNodesAdded; i < HPCParameters::TotalNumberOfNodes; i++) {
        nodes.push_back(new Node());
        numberOfNodesAdded++;
    }
    for (auto &node : nodes) {
        scheduler->addFreeNode(this, node);
        node->addScheduler(scheduler);
    }

    cout << numberOfNodesAdded << "nodes added to the scheduler \n";


    for (User *user : users) {
        user->addScheduler(scheduler);
        insert(user);
    }
    cout << users.size() << " users inserted in the timeline \n";
    doAllEvents();

    // free the memory, note that events is freed in the base class destructor
    for (int i = 0; i < nodes.size(); ++i) {
        Node *node = nodes.back();
        nodes.pop_back();
        delete node;
    }
    for (int i = 0; i < users.size(); ++i) {
        User *user = users.back();
        users.pop_back();
        delete user;
    }
    delete weekendBegin;
    delete weekendEnd;

}

void HPCSimulator::initialisation(string filename) {
    cout << "Initialising simulation from file \n";
    std::ifstream inputStream(filename);
    std::string line = "";
    getline(inputStream, line);
    //research group parsing
    while (line != "----") {
        unsigned long firstMeaningfullcharacter = line.find_first_not_of(" \t");
        if (firstMeaningfullcharacter == string::npos || line[firstMeaningfullcharacter] == '#') {
            getline(inputStream, line);
            continue;
        } else {
            double commonBudget;
            double averageTimeBetweenJobs;
            bool permissions[5];
            int numberOfResearchers;
            std::vector<double> individualGrants;

            //budget line:
            commonBudget = std::stod(line.substr(line.find(' ') + 1, line.size() - 1));
            auto *group = new Group(commonBudget);
            initialUsersBudget += commonBudget;

            // permission line :
            getline(inputStream, line);
            int indexFirstSpace = line.find(' ');
            for (int i = 0; i < 5; ++i) {
                permissions[i] = ('1' == line[indexFirstSpace + 1 + 2 * i]);
            }

            //Average Time Between Two Jo1bs line
            getline(inputStream, line);
            averageTimeBetweenJobs = std::stod(line.substr(line.find(' ') + 1, line.size() - 1));

            //Get Individual Grant
            getline(inputStream, line);
            int indexNextSpaceIndex = line.find(' ');
            if (indexNextSpaceIndex != line.npos) {
                std::string grantsString = (line.substr(line.find(' ') + 1, line.size() - 1));
                if (grantsString.find_first_not_of(" \t") != grantsString.npos) {
                    while (!grantsString.empty()) {
                        individualGrants.push_back(std::stod(grantsString.substr(0, grantsString.find(' '))));
                        if (grantsString.find(' ') == grantsString.npos) {
                            grantsString = "";
                        } else {
                            grantsString = grantsString.substr(grantsString.find(' ') + 1, grantsString.npos - 1);
                        }

                    }
                }
            }

            // Number of researchers line
            getline(inputStream, line);
            numberOfResearchers = std::stoi(line.substr(line.find(' ') + 1, line.size() - 1));

            for (int j = 0; j < numberOfResearchers; ++j) {
                auto *researcher = new Researcher(group, averageTimeBetweenJobs);
                if (j < individualGrants.size()) {
                    initialUsersBudget += individualGrants[j];
                    researcher->addIndividualGrant(individualGrants[j]);
                }
                researcher->setPermission(permissions[0], permissions[1], permissions[2], permissions[3],
                                          permissions[4]);
                users.push_back(researcher);
            }


            cout << " Group : " << commonBudget << ',' << permissions[0] << permissions[1] << permissions[2]
                 << permissions[3] << permissions[4] << ',' << averageTimeBetweenJobs << "," << numberOfResearchers
                 << "\n";
            getline(inputStream, line);
        }
    }
    getline(inputStream, line);
    //Curriculums parsing
    while (line != "----") {
        unsigned long firstMeaningfullcharacter = line.find_first_not_of(" \t");
        if (firstMeaningfullcharacter == string::npos || line[firstMeaningfullcharacter] == '#') {
            getline(inputStream, line);
            continue;
        } else {
            double cummulativeCap;
            int instantanousCap;
            double averageTimeBetweenJobs;
            bool permissions[5];
            int numberOfStudents;
            std::vector<double> individualGrants;

            //cumulative cap line
            cummulativeCap = std::stod(line.substr(line.find(' ') + 1, line.size() - 1));

            // instantaneous cap line
            getline(inputStream, line);
            instantanousCap = std::stoi(line.substr(line.find(' ') + 1, line.size() - 1));

            auto *curriculum = new Curriculum(cummulativeCap, instantanousCap);

            // permission line :
            getline(inputStream, line);
            int indexFirstSpace = line.find(' ');
            for (int i = 0; i < 5; ++i) {
                permissions[i] = ('1' == line[indexFirstSpace + 1 + 2 * i]);
            }

            //Average Time Between Two Jobs line
            getline(inputStream, line);
            averageTimeBetweenJobs = std::stod(line.substr(line.find(' ') + 1, line.size() - 1));

            // Number of students line
            getline(inputStream, line);
            numberOfStudents = std::stoi(line.substr(line.find(' ') + 1, line.size() - 1));
            initialUsersBudget += numberOfStudents * cummulativeCap;

            for (int j = 0; j < numberOfStudents; ++j) {
                Student *student = new Student(curriculum, averageTimeBetweenJobs);
                student->setPermission(permissions[0], permissions[1], permissions[2], permissions[3], permissions[4]);
                users.push_back(student);
            }

            cout << " Curriculum : " << cummulativeCap << "," << instantanousCap << ',' << permissions[0]
                 << permissions[1] << permissions[2]
                 << permissions[3] << permissions[4] << ',' << averageTimeBetweenJobs << "," << numberOfStudents
                 << "\n";
            getline(inputStream, line);
        }
    }
}

void HPCSimulator::registerFinishedGpuJobs(GpuJob *pJob) {
    finishedGpuJobs.insert(pJob);
}

void HPCSimulator::registerFinishedSmallJobs(SmallJob *pJob) {
    finishedSmallJobs.insert(pJob);
}

void HPCSimulator::registerFinishedMediumJobs(MediumJob *pJob) {
    finishedMediumJobs.insert(pJob);
}

void HPCSimulator::registerFinishedLargeJobs(LargeJob *pJob) {
    finishedLargeJobs.insert(pJob);
}

void HPCSimulator::registerFinishedHugeJobs(HugeJob *pJob) {
    finishedHugeJobs.insert(pJob);
}

void HPCSimulator::printResults() {
    int numberOfHoursInAWeek = 168;
    double numberOfWeeks = floor(time / numberOfHoursInAWeek);
    double nodeHoursUsed = 0, nodeHoursUsedBySmall = 0, nodeHoursUsedByMedium = 0, nodeHoursUsedByLarge = 0, nodeHoursUsedByHuge = 0, nodeHoursUsedByGpu = 0;
    double averageWaitingTimeSmall = 0, averageWaitingTimeMedium = 0, averageWaitingTimeLarge = 0, averageWaitingTimeHuge = 0, averageWaitingTimeGpu = 0;
    std::vector<double> turnaroudTimeRatio;
    double averageTurnaroundTimeRatio = 0;
    double opertationCost = HPCParameters::overallOperationCostPerHour * (numberOfWeeks * numberOfHoursInAWeek);

    cout << "\nThe simulation ran for : " << numberOfWeeks << " weeks \n";
    cout << "==============THROUGHPUT==============\n";
    cout << "In average, " << finishedSmallJobs.size() / numberOfWeeks << " small jobs ran per weeks \n";
    cout << "In average, " << finishedMediumJobs.size() / numberOfWeeks << " medium jobs ran per weeks \n";
    cout << "In average, " << finishedLargeJobs.size() / numberOfWeeks << " large jobs ran per weeks \n";
    cout << "In average, " << finishedHugeJobs.size() / numberOfWeeks << " huge jobs ran per weeks \n";
    cout << "In average, " << finishedGpuJobs.size() / numberOfWeeks << " gpu jobs ran per weeks \n";

    for (auto &job: finishedSmallJobs) {
        nodeHoursUsedBySmall += job->getExecutionDuration()*job->getNumberOfNodes();
        averageWaitingTimeSmall += (job->getCompletionTime() - job->getSubmittingTime() - job->getExecutionDuration());
        turnaroudTimeRatio.push_back(
                (job->getCompletionTime() - job->getSubmittingTime()) / job->getExecutionDuration());
    }
    averageWaitingTimeSmall = averageWaitingTimeSmall / finishedSmallJobs.size();
    for (auto &job: finishedMediumJobs) {
        nodeHoursUsedByMedium += job->getExecutionDuration()*job->getNumberOfNodes();
        averageWaitingTimeMedium += (job->getCompletionTime() - job->getSubmittingTime() - job->getExecutionDuration());
        turnaroudTimeRatio.push_back(
                (job->getCompletionTime() - job->getSubmittingTime()) / job->getExecutionDuration());
    }
    averageWaitingTimeMedium = averageWaitingTimeMedium / finishedMediumJobs.size();
    for (auto &job: finishedLargeJobs) {
        nodeHoursUsedByLarge += job->getExecutionDuration()*job->getNumberOfNodes();
        averageWaitingTimeLarge += (job->getCompletionTime() - job->getSubmittingTime() - job->getExecutionDuration());
        turnaroudTimeRatio.push_back(
                (job->getCompletionTime() - job->getSubmittingTime()) / job->getExecutionDuration());
    }
    averageWaitingTimeLarge = averageWaitingTimeLarge / finishedLargeJobs.size();
    for (auto &job: finishedHugeJobs) {
        nodeHoursUsedByHuge += job->getExecutionDuration()*job->getNumberOfNodes();
        averageWaitingTimeHuge += (job->getCompletionTime() - job->getSubmittingTime() - job->getExecutionDuration());
        turnaroudTimeRatio.push_back(
                (job->getCompletionTime() - job->getSubmittingTime()) / job->getExecutionDuration());
    }
    averageWaitingTimeHuge = averageWaitingTimeHuge / finishedHugeJobs.size();
    for (auto &job: finishedGpuJobs) {
        nodeHoursUsedByGpu += job->getExecutionDuration()*job->getNumberOfNodes();
        averageWaitingTimeGpu += (job->getCompletionTime() - job->getSubmittingTime() - job->getExecutionDuration());
        turnaroudTimeRatio.push_back(
                (job->getCompletionTime() - job->getSubmittingTime()) / job->getExecutionDuration());
    }
    averageWaitingTimeGpu = averageWaitingTimeGpu / finishedGpuJobs.size();

    nodeHoursUsed = nodeHoursUsedByGpu + nodeHoursUsedByHuge + nodeHoursUsedByLarge + nodeHoursUsedByMedium +
                    nodeHoursUsedBySmall;

    for (auto &ratio : turnaroudTimeRatio) {
        averageTurnaroundTimeRatio += ratio;
    }
    averageTurnaroundTimeRatio = averageTurnaroundTimeRatio / turnaroudTimeRatio.size();

    cout << "\n============NODE-HOURS USED============\n";
    cout << nodeHoursUsed << " node-hours have been used on " << initialUsersBudget << " available\n"
         << nodeHoursUsedBySmall << " for small jobs \n"
         << nodeHoursUsedByMedium << " for medium jobs \n"
         << nodeHoursUsedByLarge << " for large jobs \n"
         << nodeHoursUsedByHuge << " for huge jobs \n"
         << nodeHoursUsedByGpu << " for Gpu jobs \n"
         << " The utilization ratio is " << nodeHoursUsed / (numberOfHoursInAWeek * numberOfWeeks) << "\n"
         << "'(number of node-hours used / number of node-hours available on the HPC)";


    cout << "\n================ COSTS ================\n";
    double totalUserCost = nodeHoursUsed * HPCParameters::costOneHourOneNode + (nodeHoursUsedByGpu *
                                                                                (HPCParameters::costOneHourOneGPUNode -
                                                                                 HPCParameters::costOneHourOneNode));
    cout << "Resulting price paid by users :" << totalUserCost << "\n"
         << nodeHoursUsedBySmall * HPCParameters::costOneHourOneNode << " for small jobs \n"
         << nodeHoursUsedByMedium * HPCParameters::costOneHourOneNode << " for medium jobs \n"
         << nodeHoursUsedByLarge * HPCParameters::costOneHourOneNode << " for large jobs \n"
         << nodeHoursUsedByHuge * HPCParameters::costOneHourOneNode << " for huge jobs \n"
         << nodeHoursUsedByGpu * HPCParameters::costOneHourOneGPUNode << " for Gpu jobs \n";


    cout << "\n============ WAITING TIME ============\n"
         << "Average waiting time in queue : \n"
         << averageWaitingTimeSmall << " for small jobs \n"
         << averageWaitingTimeMedium << " for medium jobs \n"
         << averageWaitingTimeLarge << " for large jobs \n"
         << averageWaitingTimeHuge << " for huge jobs \n"
         << averageWaitingTimeGpu << " for Gpu jobs \n";
    cout << "Average turnaround time ratio : " << averageTurnaroundTimeRatio << "\n";


    cout << "\n=========== ECONOMIC BALANCE ==========\n";
    cout << "Economic balance of the center : " << totalUserCost - opertationCost << "\n";
}




	

