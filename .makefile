runWithExample: compile
	cd data; ../bin/SuperComputerSimulation.out InputData.txt

compile:
	g++ -std=c++11 src/* -o ./bin/SuperComputerSimulation.out

