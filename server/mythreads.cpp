#include "mythreads.hpp"

void* workerThread(void* args)
{
    MyQueue* mq = (MyQueue*)args;   //get the arguments
    while(true) //keep running
    {
        MyJob* mj = mq->popTop();   //get a job
        if(mj == NULL)  //if it's empty
            break;  //stop running
        mj->sendFileToClient(); //else, send it to client
        delete mj;  //delete the job
    }
    pthread_exit(NULL); //exit
}

void* firstLineThread(void* args)
{
    MyThreadArgs* mta = (MyThreadArgs*) args;   //get the arguments
    int sock = mta->getSock();  //set them to local variables
    MyQueue* mq = mta->getQueue();

    char buff[PATH_MAX + 1];    //set a buffer for the name
    memset(buff, '\0', PATH_MAX + 1);   //initialise it
    readName(sock, buff);   //read the name of the request

    if(!mq->increaseFirstLineThreads()) //if you can, increase the number of workerThreads
    {   //if not, it means it's no longer active, so
        sendNumber(sock, 0);    //send a negative response
        sendNumber(sock, 0);
        if(close(sock) == -1)   //close the socket
            perror("close");
        delete mta;             //free the memory
        pthread_detach(pthread_self());     //detach the thread
        pthread_exit(NULL);             //and exit
    }

    if(strcmp(buff, "exit") == 0)   //if you receive exit command
    {
        sendNumber(sock, 0);    //send negative response
        sendNumber(sock, 0);
        mq->deactivate();   //deactivate the queue
        mq->decreaseFirstLineThreads(); //decrease the active worker threads
        if(close(sock) == -1)   //close the socket
            perror("close");
        pthread_t pt;   //create the terminator
        pthread_create(&pt, NULL, terminator, args);
        pthread_detach(pthread_self()); //detach yourself
        pthread_exit(NULL); //and exit
    }

    delete mta; //free the memory of arguments you no longer need

    MyINodeList myFiles, myDirectories;
    if(buff[0] == '/')  //check if the path is relative or absolute
        getDirectoryItems(buff, "", &myFiles, &myDirectories);  //and fill the lists with files and directories
    else
        getDirectoryItems(buff, "./", &myFiles, &myDirectories);

    int numberOfFiles = myFiles.length();       //get the number of files contained
    int numberOfDirectories = myDirectories.length();   //and the number of directories

    std::cout << "[" << pthread_self() << "]: " << myDirectories.length() << " directories containing " << myFiles.length() << " files to be sent to: " << sock << std::endl << std::endl;
    //let the user know
    sendNumber(sock, numberOfDirectories);  //send the number of directories to the client
    sendNumber(sock, numberOfFiles);        //and the number of files

    while(!myDirectories.isEmpty()) //send the directories to the client
    {
        sendDirectory(sock, myDirectories.getTop());
        myDirectories.removeTopEntry();
    }

    pthread_mutex_t* m = new pthread_mutex_t;   //create a mutex for this client
    int* i = new int;                           //and a counter for jobs done
    *i = 0;
    pthread_mutex_init(m, NULL);        //initialize the mutex

    while(!myFiles.isEmpty())
    {
        MyINode* mind = myFiles.popTop();   //print info
        std::cout << "[" << pthread_self() << "]: " << "Adding <" << mind->getName() << ", " << sock << "> to queue." << std::endl;
        MyJob* mj = new MyJob(mind, sock, m, i, numberOfFiles); //create jobs
        mq->insertEntry(mj);    //add them to the queue
    }

    mq->decreaseFirstLineThreads();     //decrease active FirstLineThreads
    pthread_detach(pthread_self());     //detach yourself
    pthread_exit(NULL);                 //and exit
}

void* terminator(void* args)
{
    std::cout << "I'll be back!" << std::endl;  //couldn't help it
    MyThreadArgs* mta = (MyThreadArgs*)args;    //get the arguments
    int sock = mta->getMainSock();          //store them locally
    MyQueue* mq = mta->getQueue();
    pthread_t* workers = mta->getWorkerThreads();
    int workersAmount = mta->getWorkersAmount();
    pthread_t mainThread = mta->getMainThread();

    mq->waitForActiveThreads(); //wait for all active firstLineThreads to finish
    if(close(sock) == -1)   //close the main socket
        perror("close");

    for(int i = 0; i < workersAmount; i++)
    {   //join all worker threads
        mq->signalWorkersForExit();
        pthread_join(workers[i], NULL);
    }

    pthread_kill(mainThread, SIGRTMIN+1);   //signal the main thread to exit
    pthread_join(mainThread, NULL); //join it

    delete mq;  //delete all allocated memory
    delete[] workers;
    delete mta;

    pthread_detach(pthread_self()); //detach yourself
    exit(0);    //and exit
}
