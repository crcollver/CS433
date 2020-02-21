// =================================
// CS 433 Programming Assignment 4
// Author: Cameron Collver and Nick Dujakovich
// Date: 11/20/2019
// Course: CS433 (Operating Systems)
// Description : Solving Producer-Consumer problem using Pthreads API
// =================================

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


const int bufferSize = 5; //delcaring size of buffer

//defining buffer item type to int
typedef int buffer_item;

//Mutex/Semaphore declaration
pthread_mutex_t mutex;
sem_t full;
sem_t empty;

//declaring buffer
buffer_item buffer[bufferSize];
int head = 0;       //head of queue
int tail = 0;       //tail of queue
int counter = 0;    //queue counter

// function declarations
// just personal preference
void printBuffer();
bool insertItem(buffer_item item);
bool removeItem();
void *producer(void *param);
void *consumer(void *param);

int main(int argc, char* argv[]) {
    //Print basic information about the program
	std::cout << "=================================================================" << std::endl;
	std::cout << "CS 433 Programming assignment 4" << std::endl;
	std::cout << "Author: Cameron Collver and Nick Dujakovich" << std::endl;
	std::cout << "Date: 11/20/2019" << std::endl;
	std::cout << "Course: CS433 (Operating Systems)" << std::endl;
	std::cout << "Description : Solving Producer-Consumer problem using Pthreads API" << std::endl;
	std::cout << "=================================================================\n" << std::endl;

    //Error handling
    if(argc != 4) {
        std::cout << "Error. Please execute the program in the following format:" << std::endl;
        std::cout << "./prog4 SLEEP_TIME_IN_SECONDS NUM_OF_PRODUCERS NUM_OF_CONSUMERS" << std::endl;;
        return -1;
    }

    // seed random number generator
    srand(time(NULL));
    
    //Input assignment
    int sleepTime = atoi(argv[1]);          //1st argument assigned to sleep time
    int numProducers = atoi(argv[2]);       //2nd argument assigned to number of producer threads
    int numConsumers = atoi(argv[3]);       //3rd argument assigned to number of consumer threads

    //std::cout << sleepTime << numProducers << numConsumers << std::endl;

    //initialize mutex/semaphores/threads
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, bufferSize);

    // each thread must have unique id, so we create arrays
    pthread_t producer_tid[numProducers]; // array for producer ids
    pthread_t consumer_tid[numConsumers]; // array for consumer ids     

    for(int i = 0; i < numProducers; i++) {
        // attribute declarations for our threads
        pthread_attr_t attr;
        pthread_attr_init(&attr);   
        pthread_create(&producer_tid[i], &attr, producer, NULL);        //creates Producer threads
    }
    for(int i = 0; i < numConsumers; i++) {
        // attribute declarations for our threads
        pthread_attr_t attr;
        pthread_attr_init(&attr);   
        pthread_create(&consumer_tid[i], &attr, consumer, NULL);        //creates Consumer threads
    }
    
    sleep(sleepTime);   //thread sleeps for sleepTime (seconds) until terminating
    
    return 0;
}

//Prints buffer from start of head
void printBuffer() {
    std::cout << "The current content of the buffer is [ ";
    int index = head; // get the front of the queue
    // loop through amount of items in queue
    for(int i = 0; i < counter; i++) {
        std::cout << buffer[index] << " ";
        index = (index + 1) % bufferSize; // if index was 5, then get item at 0 - circular
    }
    std::cout << "]" << std::endl << std::endl;
}

//Inserts item into the end of buffer
bool insertItem(buffer_item item) {
    if(counter < bufferSize) {          //if current size of buffer < buffer limit, proceed
        buffer[tail] = item;            //adds item to tail of buffer
        tail = (tail + 1) % bufferSize; //increments tail - circular
        counter++;                      //increments size of buffer
        std::cout << "item " << item << " inserted by a producer." << std::endl;
        return true;                    //returns true if completed
    }
    return false;                       //returns false if buffer is full
}

//Removes item from front of buffer
bool removeItem() {
    if(counter > 0) {
        buffer_item item = buffer[head];    //stores head of buffer to item
        counter--;                          //decrements size of buffer
        head = (head + 1) % bufferSize;     //increments head - circular
        std::cout << "item " << item << " removed by a consumer." << std::endl;
        return true;                        //returns true if completed
    }
    return false;                           //returns false if buffer is empty
}

//Producer
void *producer(void *param) {
    buffer_item item;                       //declares item of type buffer_item
    while(true) {                           //keeps running until thread terminates
        sleep((rand() % 5 + 1));            //sleep for a random time between 1-5 seconds
        item = rand() % 100 + 1;            //give item a random number between 1-100

        sem_wait(&empty);                   //decrements empty semaphore if empty > 0
        pthread_mutex_lock(&mutex);         //acquires mutex lock

        if(!insertItem(item)) {             //inserts item into buffer; returns True or False
            std::cout << "Error. Buffer full." << std::endl;    //error handling
        } else {
            printBuffer();                  //prints Buffer if insertion is successful
        }

        pthread_mutex_unlock(&mutex);       //releases mutex lock
        sem_post(&full);                    //increments full semaphore
    }

}


//Consumer
void *consumer(void *param) {
    while(true) {                           //keeps running until thread terminates
        sleep((rand() % 5 + 1));            //sleep for a random time between 1-5 seconds

        sem_wait(&full);                    //decrements full semaphore if full > 0
        pthread_mutex_lock(&mutex);         //acquires mutex lock

        if(!removeItem()) {                 //removes item from head of buffer; return True or False
            std::cout << "Error. Buffer empty." << std::endl;   //error handling
        } else {
            printBuffer();                  //prints Buffer if removal is successful
        }

        pthread_mutex_unlock(&mutex);       //release mutex lock
        sem_post(&empty);                   //increments empty semaphore
    }

}