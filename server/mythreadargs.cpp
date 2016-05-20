#include "mythreadargs.hpp"

MyThreadArgs::MyThreadArgs(MyQueue* q, int s, int ms, pthread_t* p, int w, pthread_t mt): mq(q), sock(s), mainSock(ms),  workerThreads(p), workers(w), mainThread(mt) {}

MyThreadArgs::~MyThreadArgs(){}

MyQueue* MyThreadArgs::getQueue() {return mq;}

int MyThreadArgs::getSock() {return sock;}

int MyThreadArgs::getMainSock() {return mainSock;}

pthread_t* MyThreadArgs::getWorkerThreads() {return workerThreads;}

int MyThreadArgs::getWorkersAmount() {return workers;}

pthread_t MyThreadArgs::getMainThread() {return mainThread;}
