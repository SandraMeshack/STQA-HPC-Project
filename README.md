#HPC Plateform Simulation

This project is created as part of an assignment for Cranfield University CSTE (option STEC) MSc. The assignment can be found in `doc/Assignment.pdf`

It aims to simulate the behavior of a computing platform for exploring different queueing and accounting strategies.

Information about the design can be found in ```doc/Description_of_Design_employed.pdf```

A Doxyfile is available at `doc/Doxyfile` for generating html and latex documentation with Doxygen.

# With Cmake
In order to configure this project CMakeFileList.txt have been configure.
The user shall run ```cmake .``` int the project folder.
Then the following commands will be available :
- ```make``` for building the project
- ```make test``` for running tests

The compiled code can be found on the ```/bin``` folder

The Simulation require an input file for configuring the simulation.
An example of config file can be found in ```/data``` 

# Without Cmake
For plateform where you can't use cmake, a handmade makefile as been povided.
In order to use rename the ```.makefile``` into ```.makefile``` at the root of this project

This makefile offers to commands : 
- ```make runWithExample``` which run the simulation with the example file
- ```make compile``` which compile the project in the `/bin` folder

  
