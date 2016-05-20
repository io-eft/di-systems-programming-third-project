#include "myjob.hpp"

MyJob::MyJob(MyINode* mi, int s, pthread_mutex_t* m, int* jd, int mj): mind(mi), sock(s), socketLock(m), jobsDone(jd), maxJobs(mj){}

MyJob::~MyJob()
{
    bool last = false;
    delete mind;    //delete the myinode

    if(*jobsDone == maxJobs)    //check if it's the last
        last = true;
    pthread_mutex_unlock(socketLock);   //unlock the mutex
    if(last)    //if it is the last
    {
        delete jobsDone;    //delete the counter
        if(pthread_mutex_destroy(socketLock))   //destroy the mutex
            std::cerr << "Error destroying the mutex!" << std::endl;
        delete socketLock;  //delete the mutex
        if(close(sock) == -1)    //and close the socket
            perror("close");
    }
}

void MyJob::sendFileToClient()
{
    pthread_mutex_lock(socketLock);     //lock the mutex
    printJob(); //print the job
    sendFile(sock, mind);   //do the job
    (*jobsDone)++;  //increase the counter
    jobDone();  //print that you are done
}   //mutex stays locks so MyJob can be destroyed and properly check if it is the last

void MyJob::printJob()
{
    std::cout << "Sending: "<< mind->getName() << " - [" << pthread_self() << " -> " << sock << "]" << std::endl;
}

void MyJob::jobDone()
{
    std::cout << "Done: "<< mind->getName() << " - [" << pthread_self() << " -> " << sock << "]" << std::endl;
}
