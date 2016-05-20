#ifndef __MYTHREADARGS__HPP
#define __MYTHREADARGS__HPP
#include "myqueue.hpp"

class MyThreadArgs{
    MyQueue* mq;
    int sock;
    int mainSock;
    pthread_t* workerThreads;
    int workers;
    pthread_t mainThread;

    public:
	MyThreadArgs(MyQueue* q, int s, int ms, pthread_t* p, int w, pthread_t mt);
	~MyThreadArgs();

	MyQueue* getQueue();
	int getSock();
	int getMainSock();
	pthread_t* getWorkerThreads();
	int getWorkersAmount();
	pthread_t getMainThread();
};

#endif
