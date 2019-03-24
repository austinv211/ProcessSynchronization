# Contents of the Zip File

## Source Files
- boundedBuffer.cpp 
  - Description: bounded buffer problem #1 implemented in C++
  - Observation: Works effectively as described in problem documentation, mutex's are crucially important when adding and removing from the buffer, since array errors can be caused by race conditions
  

- mh.cpp
  - Description: implementation of problem #2 in C++
  - Observation: Mutexes are needed to prevent printing errors in the output, if two threads try to print to std out at the same time

- airline.c
  - Description: implementation of problem #3 in C
  - Observation: Threads will repeat their routine if not given a return, a while loop and counters were able to effectively solve the issues with routines. since so many passenger threads are created compared to the available CPU threads, waiting for all the threads with pthread_join is not as effective as implementing a finished semaphor

- makefile
  - Description: makefile for the contents of the zip

- testCode.sh
  - shell code to test Code for project
  - should make executable by running make, if needed to make shell script executable run: chmod a+rx ./testCode.sh
  - code will be tested part by part, asking the user to press enter to jump to the next section

# Output Files Created by Makefile
- boundedBuffer
  - syntax: ./boundedBuffer
- mh
  - syntax: ./mh <number_of_cycles>
- airline
  - syntax: ./airline <number_of_passengers> <number_of_baggage_checkers> <number_of_security_screeners> <number_of_flight_attendants>

