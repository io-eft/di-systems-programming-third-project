#ifndef __MYQUEUE__HPP
#define __MYQUEUE__HPP
#include <iostream>
#include <string>	
#include "myjob.hpp"

class MyQueue{
    struct Node{
        MyJob* content;
        Node* next;

        Node(MyJob* mj);
        Node();
	~Node();
    };
    Node* head;
    Node* tail;

    bool status;
    
    int items;
    int itemLimit;
    int threads;
    
    pthread_mutex_t queueLock;
    //pthread_mutex_t itemsLock;
    pthread_mutex_t firstLineThreads;
    pthread_mutex_t activeStatus;

    pthread_cond_t emptyQueue;
    pthread_cond_t fullQueue;
    pthread_cond_t readyToExit;

    public:
        MyQueue(int n);
        ~MyQueue();
        bool isEmpty();
	bool isFull();

        void insertEntry(MyJob* mj);
	MyJob* popTop();

	void lockOnInsert();
	void lockOnRemove();
	void unlockOnInsert();
	void unlockOnRemove();

	bool increaseFirstLineThreads();
	void decreaseFirstLineThreads();
	bool isActive();
	void deactivate();

	void waitForActiveThreads();
	void signalWorkersForExit();

	void increaseItems();
	void decreaseItems();
};

#endif
