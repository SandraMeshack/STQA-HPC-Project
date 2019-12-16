
#include "../include/HPCSimulator.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "You should provide the simulation programm with one argument : \n"
                   <<"the path to the file with the scenario \n";
        return 1;
    }
    cout << " HPC simulator initialisation" << std::endl;
    HPCSimulator hpcSimulator;
    hpcSimulator.initialisation("../data/InputDataExample.txt");
    cout << " Starting" << std::endl;
    hpcSimulator.start();
    hpcSimulator.printResults();
    return 0;
}
