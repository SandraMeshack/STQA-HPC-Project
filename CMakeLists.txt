cmake_minimum_required(VERSION 3.15)
project(SuperComputerSimulation)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES ./src/main.cpp src/AbstractSimulator.cpp src/ListQueue.cpp src/AbstractJob.cpp src/Simulator.cpp src/HPCSimulator.cpp src/Node.cpp src/AbstractScheduler.cpp include/User.h src/User.cpp src/Curriculum.cpp include/Curriculum.h src/Curriculum.cpp src/Student.cpp src/Student.cpp include/Student.h src/weekendEvent.cpp include/weekendEvent.h src/HPCParameters.cpp include/HPCParameters.h src/Researcher.cpp src/Group.cpp)
set(TESTS_FILES tests/tests-main.cpp tests/factorial-test.cpp)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(./tests)
endif ()

add_compile_options(-Wpedantic)

add_executable(SuperComputerSimulation ${SOURCE_FILES})