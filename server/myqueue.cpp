#include "myqueue.hpp"

MyQueue::Node::Node(): content(NULL), next(NULL) {}

MyQueue::Node::Node(MyJob* mj): content(mj), next(NULL){}

MyQueue::Node::~Node()
{
    if(content != NULL)
        delete content;
}

MyQueue::MyQueue(int n): head(NULL), tail(NULL), items(0), itemLimit(n),threads(0), status(true)
{
    pthread_mutex_init(&queueLock, NULL);       //initialize the mutexes
    //pthread_mutex_init(&itemsLock, NULL);       //initialize the mutexes
    pthread_mutex_init(&firstLineThreads, NULL);
    pthread_mutex_init(&activeStatus, NULL);

    pthread_cond_init(&emptyQueue, NULL);   //and the condition variables
    pthread_cond_init(&fullQueue, NULL);
    pthread_cond_init(&readyToExit, NULL);
}

MyQueue::~MyQueue()
{
    Node* temp;
    while(head != NULL) //delete antything that's left
    {
        temp = head;
        head = head->next;
        delete temp;
    }

    pthread_mutex_destroy(&queueLock);  //and destroy the mutexes
    //pthread_mutex_destroy(&itemsLock);  //and destroy the mutexes
    pthread_mutex_destroy(&firstLineThreads);
    pthread_mutex_destroy(&activeStatus);

    pthread_cond_destroy(&emptyQueue);  //and the condition variables
    pthread_cond_destroy(&fullQueue);
    pthread_cond_destroy(&readyToExit);
}

bool MyQueue::isEmpty()
{
    /*bool result;
    pthread_mutex_lock(&itemsLock);
    if(items == 0)
        result = true;
    else
        result = false;
    pthread_mutex_unlock(&itemsLock);
    return result;*/
    if(items == 0)
        return true;
    else
        return false;
}

bool MyQueue::isFull()
{
    /*bool result;
    pthread_mutex_lock(&itemsLock);
    if(items == itemLimit)
        result = true;
    else
        result = false;
    pthread_mutex_unlock(&itemsLock);
    return result;*/

    if(items == itemLimit)
        return true;
    else
        return false;
}
/*
void MyQueue::increaseItems()
{
    pthread_mutex_lock(&itemsLock);
    items++;
    std::cout << items << std::endl;
    pthread_mutex_unlock(&itemsLock);
}

void MyQueue::decreaseItems()
{
    pthread_mutex_lock(&itemsLock);
    items--;
    pthread_mutex_unlock(&itemsLock);
}*/

void MyQueue::insertEntry(MyJob* mj)
{
    lockOnInsert();     //lock access to the queue
    Node* n = new Node(mj); //create a new node
    if(isEmpty())   //if it's empty
    {
        head = n;   //set the head and the tail to point at it
        tail = n;
        pthread_cond_broadcast(&emptyQueue);    //and singal the workers
    }
    else    //if it's not empty
    {
        tail->next = n; //set it as the last one
        tail = n;
    }
    items++;
    //increaseItems();
    pthread_mutex_unlock(&queueLock);   //unlock the mutex
}

MyJob* MyQueue::popTop()
{
    lockOnRemove(); //lock the access to the queue

    if(isEmpty() && !isActive())    //if the queue is empty and not active
    {
        pthread_mutex_unlock(&queueLock);   //unlock the mutex
        return NULL;    //and return NULL
    }

    MyJob* temp = head->content;    //get the head
    Node* toBeDeleted = head;   //set it to be deleted
    head = head->next;  //set as new head the following one
    if(isFull())    //if the queue is full
        pthread_cond_broadcast(&fullQueue); //signal the firstLineThreads
    items--;    //reduce the number of items
    //decreaseItems();
    toBeDeleted->content = NULL;    //set the content of the node as NULL, so it's not deleted
    delete toBeDeleted; //delete the node
    pthread_mutex_unlock(&queueLock);   //unlock the mutex
    return temp;    //return the job
}

void MyQueue::lockOnInsert()
{
    pthread_mutex_lock(&queueLock); //lock the mutex
    while(isFull()) //while it's full
        pthread_cond_wait(&fullQueue, &queueLock);  //wait on the condition variable
}

void MyQueue::lockOnRemove()
{
    pthread_mutex_lock(&queueLock); //lock the mutex
    while(isEmpty())   //while queue is empty
    {
        if(!isActive()) //if it's not active break
            break;
        pthread_cond_wait(&emptyQueue, &queueLock); //wait on the condition variable
    }
}

bool MyQueue::increaseFirstLineThreads()
{
    bool result = true;
    pthread_mutex_lock(&firstLineThreads);  //get the mutex for first line threads
    if(!isActive())     //if it's not active
        result = false; //let the caller know
    else
        threads++;  //else, increase the number of threads
    pthread_mutex_unlock(&firstLineThreads);    //unlock the mutex
    return result;  //and return the result
}

void MyQueue::decreaseFirstLineThreads()
{
    pthread_mutex_lock(&firstLineThreads);  //get the mutex for first line threads
    threads--;      //decraese the number of them
    if(threads == 0 && !isActive()) //if it's the last one, and the queue is no longer active
        pthread_cond_broadcast(&readyToExit);   //wake the terminator
    pthread_mutex_unlock(&firstLineThreads);    //unlock the mutex
}

bool MyQueue::isActive()
{
    bool result;
    pthread_mutex_lock(&activeStatus);  //get the activity mutex
        result = status;    //get the value
    pthread_mutex_unlock(&activeStatus);    //unlock the mutex
    return result;  //return the value
}

void MyQueue::deactivate()
{
    pthread_mutex_lock(&activeStatus);  //get the activity mutex
    status = false;                     //deactivate it
    pthread_mutex_unlock(&activeStatus);    //unlock the mutex
}

void MyQueue::waitForActiveThreads()
{
    pthread_mutex_lock(&firstLineThreads);  //get the mutex for first line threads
    while(threads > 0)      //while there are still active threads
        pthread_cond_wait(&readyToExit, &firstLineThreads); //wait for them to finish
    pthread_mutex_unlock(&firstLineThreads);
}

void MyQueue::signalWorkersForExit()
{
    pthread_cond_broadcast(&emptyQueue);    //wake up all workers so they can exit
}
