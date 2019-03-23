#include<iostream>
#include<fstream>
#include<cstdlib>
#include<pthread.h>
#include<semaphore.h>
#include<string>
#include<vector>
#include<random>
#include<ctime>

using namespace std;

//Global Declaration(s)
const int MAX = 3;
int consumedCount = 0;
vector<char>buffer;
fstream myFile;

//Set up Mutex and semaphore(s)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t produce;
sem_t consume;

//Prototype(s)
void* producer(void* arg);
void* consumer(void* arg);

int main()
{
    //Declation(s)
    //myFile.open("test.txt", ios::out | ios::app);
    
    //Set value for semaphore(s)
    sem_init(&produce, 0, 1);
    sem_init(&consume, 0, 1);

    //Create First thread
    //int num1 = 1;
    pthread_t id1;
    pthread_create(&id1, NULL, producer, NULL);

    //Create second thread
    //int num2 = -1;
    pthread_t id2;
    pthread_create(&id2, NULL, consumer, NULL);

    //Join Thread(s)
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    //Destory Semephores
    sem_destroy(&produce);
    sem_destroy(&consume);

    //Close file
    //myFile.close();
	return 0;
}

//Producer Function
void* producer(void* arg){
    
    //Wait for signal
    sem_wait(&produce);
    sem_post(&consume);

    //Seed for random number generator
    srand(time(0));

    //Start Producing
    while(consumedCount != 6){

        //Produce a letter for the buffer
        int randNum = rand() % 26 + 97;
        char dummy = static_cast<char>(randNum);

        //Do nothing while buffer is full
        while(buffer.size() == MAX)
        {/*myFile << "Full\n";*/}

        //Lock Mutex and adds letter to buffer
        pthread_mutex_lock(&mutex);
        cout << "Producer locked mutex: Adding '" <<  dummy << "' to buffer.\n";
        buffer.push_back(dummy);

        //Unlock Mutex
        pthread_mutex_unlock(&mutex);
        cout << "Producer unlocked mutex.\n";
    }
}

//Consumer Function
void* consumer(void* arg){

    //Wait for signal
    sem_wait(&consume);
    
    //Start consuming
    while(consumedCount != 6){

        //Do nothing while buffer is empty 
        while(buffer.size() == 0)
        {/*myFile << "empty\n";*/}

        //Lock Mutex, remove and print letter from buffer
        pthread_mutex_lock(&mutex);
        cout << "Consumer locked mutex: Removing '" << buffer[0] << "' from buffer.\n";
        buffer.erase(buffer.begin() + 0);
        consumedCount++;

        //Unlock Mutex
        pthread_mutex_unlock(&mutex);
        cout << "Consumer unlocked mutex.\n";
    }
}