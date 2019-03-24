#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>

//nums to be initialized in main
int numPassengers, numBaggageCheckers, numSecurityScreeners, numFlightAttendants;

//lock for printing
pthread_mutex_t printer = PTHREAD_MUTEX_INITIALIZER;

//nums to work as counter
int PCB = 0;
int PCS = 0;
int PCF = 0; 
int PCP = 0;

//define semaphors to work with
sem_t baggageCheck, securityCheck, flightAttendantCheck, finished;

//flight routine to be performed by the flight attendant threads
void* flightRoutine(void* arg) {

    // loop through, while the number of passengers processed by the flight attendants is less than the num passengers
    while (PCF < numPassengers) {
        //wait for the security routine to have a passenger ready
        sem_wait(&flightAttendantCheck);

        //lock the printer mutex to prevent printing and incrementing problems
        pthread_mutex_lock(&printer);

        //increase the count of flight attendant processed passengers
        PCF++;

        //set the integer to be printed to PCF
        int i = PCF;

        //print the step
        printf("Passenger #%d has now been seated.\n", i);

        //unlock the printer mutex
        pthread_mutex_unlock(&printer);
        
    }

    //post to the finished mutex to signify that all passengers have been seated
    sem_post(&finished);

    //return null as the end of the routine
    return NULL;
}


//routine that the security check threads perform
void* securityRoutine(void* arg) {

    // loop while the number of passengers processed by security is less than the number of passengers
    while (PCS < numPassengers) {

        //wait for the baggage checkers to have a passenger ready
        sem_wait(&securityCheck);

        //lock the printer mutex to prevent issues with printing and incrementing
        pthread_mutex_lock(&printer);

        //increment the count processed by security
        PCS++;

        //set the digit to be printed to PCS
        int i = PCS;

        //print the step
        printf("Passenger #%d waiting for security screening.\n", i);
        
        //unlock the printer mutex
        pthread_mutex_unlock(&printer);

        //let the flight attendant know that a passenger is ready to get seated on the plane
        sem_post(&flightAttendantCheck);
    }
    
    //return NULL as the end of the routine
    return NULL;
}

//routine for the baggage checker threads to perform
void* baggageRoutine(void* arg) {

    //loop while the number of passengers baggage checked is less than the number of passengers
    while (PCB < numPassengers) {

        //wait for the passenger threads to let the baggage checkers know they can start checking baggage
        sem_wait(&baggageCheck);

        //lock the printer mutex to prevent issues with printing and incrementing
        pthread_mutex_lock(&printer);

        //increase the passenger count processed by baggage check
        PCB++;

        //set i to be the number processed by baggage check
        int i = PCB;

        //print the step
        printf("Passenger #%d waiting for bag to be checked.\n", i);

        //unlock the printer mutex
        pthread_mutex_unlock(&printer);

        //let the security check know that a passenger is ready to get security screened
        sem_post(&securityCheck);
    }

    //return NULL as the end of the routine
    return NULL;
}

//passenger routine that the passenger threads perform
void* passengerRoutine(void* arg) {

    //lock the printer mutex
    pthread_mutex_lock(&printer);

    //increase the count of passengers processed by the passenger threads
    PCP++;

    //set the i to be count of passengers processed
    int i = PCP;

    //print the step
    printf("Passenger #%d has arrived at the terminal\n", i);

    //unlock the printer mutex
    pthread_mutex_unlock(&printer);

    //increase the semaphor to let baggage check know a passenger is ready
    sem_post(&baggageCheck);

    //return NULL as the end of the routine
    return NULL;  
}

int main(int argc, char* argv[]) {

    //get the number of args, and exit if incorrect
    if (argc != 5) {
        fprintf(stderr, "Incorrect number of args.\n");
        return 1;
    }

    //get the number of threads using the commandline args and strtol, with error handling
    for (int i = 1; i < 5; i++) {
        char *end;
        long repLong = strtol(argv[i], &end, 10);

        if (end == argv[i]) {
            fprintf(stderr, "Arg provided for arg # %d is not a valid decimal number\n", i);
            return 1;
        }
        else if(*end != '\0') {
            fprintf(stderr, "Invalid characters in arg # %d for numPassengers.\n", i);
            return 1;
        }
            
        switch (i)
        {
            case 1:
                numPassengers = (int)repLong;
                break;
            case 2:
                numBaggageCheckers = (int)repLong;
                break;
            case 3:
                numSecurityScreeners = (int)repLong;
                break;
            case 4:
                numFlightAttendants = (int)repLong;
                break;                  
            default:
                break;
        }
    }

    //print the initial counts to the screen
    printf("Number of Passengers: %d\n", numPassengers);
    printf("Number of Baggage Checker: %d\n", numBaggageCheckers);
    printf("Number of Security Screeners: %d\n", numSecurityScreeners);
    printf("Number of Flight Attendants: %d\n", numFlightAttendants);
    printf("--------------------------\nTotal number of Threads: %d\n--------------------------\n\n",
     numPassengers + numBaggageCheckers + numSecurityScreeners + numFlightAttendants);
    

    //initialize semaphors, threads will increment and decrement for timing
    sem_init(&baggageCheck, 0, 0);
    sem_init(&securityCheck, 0, 0);
    sem_init(&flightAttendantCheck, 0, 0);
    sem_init(&finished, 0, 0);

    //define our list of threads
    pthread_t passengerThreads[numPassengers];
    pthread_t baggageCheckThreads[numBaggageCheckers];
    pthread_t securityCheckThreads[numSecurityScreeners];
    pthread_t flightAttendantThreads[numFlightAttendants];

    //create our baggage checkers
    for (int i = 0; i < numBaggageCheckers; i++) {

        pthread_create(&baggageCheckThreads[i], NULL, baggageRoutine, NULL);
    }

    //create our security threads
    for (int i = 0; i < numSecurityScreeners; i++) {
        pthread_create(&securityCheckThreads[i], NULL, securityRoutine, NULL);
    }

    //create our flight attendant threads
    for (int i = 0; i < numFlightAttendants; i++) {
        pthread_create(&flightAttendantThreads[i], NULL, flightRoutine, NULL);
    }

    //create our passenger threads
    for (int i = 0; i < numPassengers; i++) {
        pthread_create(&passengerThreads[i], NULL, passengerRoutine, (void *)&i);
    }

    //join the passenger threads
    for (int i = 0; i < numPassengers; i++) {

        pthread_join(passengerThreads[i], NULL);
    }

    //wait till we are done seating passengers
    sem_wait(&finished);

    //destroy the semaphors
    sem_destroy(&baggageCheck);
    sem_destroy(&securityCheck);
    sem_destroy(&flightAttendantCheck);

    //return 0 for all is well
    return 0;
    
}