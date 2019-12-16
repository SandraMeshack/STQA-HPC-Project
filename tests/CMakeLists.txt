set (CMAKE_RUNTIME_OUTPUT_DIRECTORY  ./bin)
set(CMAKE_CXX_STANDARD 14)

set(TESTS_FILES ./tests-main.cpp Group-test.cpp)

add_compile_options(-Wpedantic)
add_executable(SuperComputerSimulationTests ${TESTS_FILES})


add_test(
        NAME unitTest
        COMMAND SuperComputerSimulationTests
)

