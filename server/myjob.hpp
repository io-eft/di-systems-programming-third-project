#ifndef __MYJOB__HPP
#define __MYJOB__HPP
#include "myinode.hpp"
#include "sendandreceive.hpp"
#include <pthread.h>

class MyJob
{
    MyINode* mind;
    int sock;
    pthread_mutex_t* socketLock;
    int* jobsDone;
    int maxJobs;
    
    public:
	MyJob(MyINode* mi, int s, pthread_mutex_t* m, int* jd, int mj);
	~MyJob();

	void sendFileToClient();
	void printJob();
	void jobDone();
};

#endif

