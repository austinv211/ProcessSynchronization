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

void* flightRoutine(void* arg) {
    while (PCF < numPassengers) {
        sem_wait(&flightAttendantCheck);
        if (PCF <= PCS) {
            pthread_mutex_lock(&printer);
            PCF++;
            int i = PCF;
            printf("Passenger #%d has now been seated.\n", i);
            pthread_mutex_unlock(&printer);
        }
    }

    sem_post(&finished);
    return NULL;
}

void* securityRoutine(void* arg) {
    while (PCS < numPassengers) {
        sem_wait(&securityCheck);
        pthread_mutex_lock(&printer);
        PCS++;
        int i = PCS;
        printf("Passenger #%d waiting for security screening.\n", i);
        pthread_mutex_unlock(&printer);
        sem_post(&flightAttendantCheck);
    }

    return NULL;
}

void* baggageRoutine(void* arg) {

    while (PCB < numPassengers) {
        sem_wait(&baggageCheck);
        pthread_mutex_lock(&printer);
        PCB++;
        int i = PCB;
        printf("Passenger #%d waiting for bag to be checked.\n", i);
        pthread_mutex_unlock(&printer);
        sem_post(&securityCheck);
    }

    return NULL;
}

void* passengerRoutine(void* arg) {
    pthread_mutex_lock(&printer);
    PCP++;
    int i = PCP;
    printf("Passenger #%d has arrived at the terminal\n", i);
    pthread_mutex_unlock(&printer);
    //increas the semaphor
    sem_post(&baggageCheck);

    return NULL;  
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Incorrect number of args.\n");
        return 1;
    }

    numPassengers = atoi(argv[1]);
    numBaggageCheckers = atoi(argv[2]);
    numSecurityScreeners = atoi(argv[3]);
    numFlightAttendants = atoi(argv[4]);

    printf("Number of Passengers: %d\n", numPassengers);
    printf("Number of Baggage Checker: %d\n", numBaggageCheckers);
    printf("Number of Security Screeners: %d\n", numSecurityScreeners);
    printf("Number of Flight Attendants: %d\n", numFlightAttendants);
    printf("--------------------------\nTotal number of Threads: %d\n--------------------------\n\n",
     numPassengers + numBaggageCheckers + numSecurityScreeners + numFlightAttendants);
    

    //initialize semaphors
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

    sem_wait(&finished);

    sem_destroy(&baggageCheck);
    sem_destroy(&securityCheck);
    sem_destroy(&flightAttendantCheck);

    return 0;
    
}