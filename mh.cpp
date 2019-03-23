#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<fstream>
#include<string>
#include<pthread.h>
#include<semaphore.h>

using namespace std;

//Struct for children defined
struct Child{
    bool bathed = false,
         tuckedIn = false;
} children[12];

//Global Variable(s)
const int MAX = 12;                    //Max amount of children
int booksRead_sleeping_counter = 0,   //Tells us if when fater can sleep
    day = 0;                          //Counts the days
bool fatherWoke = false;              //a counter to avoid father.post multpitle times

//Set up mutex and Semiphores
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t mother;
sem_t father;

//Prototype(s)
void* motherWork(void* arg);
void* fatherWork(void* arg);
void wakeChild(Child kids[]);
void feedBreakfast(Child kids[]);
void sendToSchool(Child kids[]);
void feedDinner(Child kids[]);
void bathChild(Child kids[]);
void readBookAndTuckIn(Child kids[]);

int main(int argc, char *argv[])
{
    if(argc != 2){
        cerr << "Usage: ./mh int\n";
        return 1; 
    }
    
    int daysWanted = stoi(argv[1]);
    //Set value for semaphore(s)
    sem_init(&mother, 0, 1);   //Set to 1 since all children are sleeping
    sem_init(&father, 0, 0);   //Set to zero since no child has taken a bath

    //Create mother Thread
    pthread_t id1;
    pthread_create(&id1, NULL, motherWork, (void*) &daysWanted);

    //Create father thread
    pthread_t id2;
    pthread_create(&id2, NULL, fatherWork, &daysWanted);

    //Join thread(s)
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);


    //Destory Semephores
    sem_destroy(&mother);
    sem_destroy(&father);

    return 0;
}

//Mother Function
void* motherWork(void* arg)
{
    int daysRequested = *(int*) arg;
    
    //Task(s)
    while(day != daysRequested){
        //Wait for signal
        sem_wait(&mother);

        //check for break
        if(day == daysRequested)
            break;

        //reset father status and sleeping counter
        fatherWoke = false;
        
        //Reset Children
        for(int i = 0; i < MAX; i++)
        {
            children[i].bathed = false;
            children[i].tuckedIn = false;
        }  
        
        //Perform Task(s)
        cout << "Day " << day + 1 << "...\n";
        cout << "Mother awakes...\n";
        wakeChild(children);
        feedBreakfast(children);
        sendToSchool(children);
        feedDinner(children);
        bathChild(children);
        cout << "Mother is going to sleep...\n";
    }
}

//Father Function
void* fatherWork(void* arg)
{
    int daysRequested = *(int*) arg;
    //Wait for signal

    while(day != daysRequested){

        sem_wait(&father);
        cout << "Father is awake...\n";

        //Perform task(s)
        readBookAndTuckIn(children);

        //Add to day
        day++;

        //Signal Mother
        cout << "Father is going to Sleep...\n";
        sem_post(&mother);
    }
}

void wakeChild(Child kids[])
{    
    //Wakes all children
    for(int i = 0; i < MAX; i++){
        cout << "Mother wakes child " << i + 1 << ".\n"; 
        usleep(100);
    }
}

void feedBreakfast(Child kids[])
{
    //Feed children breakfast
    for(int i = 0; i < MAX; i++){
        cout << "Mother feeds child " << i + 1 << " breakfast.\n";
        usleep(100);
    }
}

void sendToSchool(Child kids[])
{
    //Send children to school
    for(int i = 0; i < MAX; i++){
        cout << "Mother sent child " << i + 1 << " to school.\n";
        usleep(100);
    }
}

void feedDinner(Child kids[])
{
    //Feed children dinner
    for(int i = 0; i < MAX; i++){
        cout << "Mother feeds child " << i + 1 << " dinner.\n";
        usleep(100);
    }
}

void bathChild(Child kids[])
{
    //Bath children
    for(int i = 0; i < MAX; i++){
        cout << "Mother showers child " << i + 1 << ".\n";
        children[i].bathed= true;
        usleep(100);
        
        if(fatherWoke == false){
            cout << "Waking up father...\n";
            sem_post(&father);
            fatherWoke = true;
        }
    }  
}

void readBookAndTuckIn(Child kids[])
{
    //Reset books read counter
    booksRead_sleeping_counter = 0;

    //Read book to bathed child
    while(booksRead_sleeping_counter != MAX){
        
        for(int i = 0; i < MAX; i++){
            
            //PerForm tasks if child bathed and not tucked in
            if(children[i].bathed == true && children[i].tuckedIn == false){
                cout << "Father reads to Child " << i + 1 << "\n";
                usleep(100);
                cout << "Father tucks in child " << i + 1 << "\n";
                kids[i].tuckedIn = true;
                booksRead_sleeping_counter++;
                usleep(100);
            }
        }
    }
}