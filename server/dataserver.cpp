#include "serverfunctions.hpp"
#include "mythreadargs.hpp"
#include "mythreads.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 7)   //check for errors in the input
        inputError();

    int portFlag, poolFlag, queueFlag;

    for(portFlag = 1; portFlag < argc; portFlag++)
        if(strcmp(argv[portFlag], "-p") == 0)
            break;

    for(poolFlag = 1; poolFlag < argc; poolFlag++)
        if(strcmp(argv[poolFlag], "-s") == 0)
            break;

    for(queueFlag = 1; queueFlag < argc; queueFlag++)
        if(strcmp(argv[queueFlag], "-q") == 0)
            break;

    if(portFlag == 7 || poolFlag == 7 || queueFlag == 7)
        inputError();

    int port = getNumber(argv[portFlag + 1]);
    int poolSize = atoi(argv[poolFlag + 1]);
    int queueSize = atoi(argv[queueFlag + 1]);

    signal(SIGRTMIN+1, sigHandler); //set the signal handler for termination

    int sock, newsock;              //initialize the server, as shown in the example
    struct sockaddr_in server, client;
    socklen_t clientlen;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct sockaddr *clientptr = (struct sockaddr*)&client;
    struct hostent *rem;

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)    //get the socket
    {
        perror("socket");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if(bind(sock, serverptr, sizeof(server)) < 0)   //bind it
    {
        perror("bind");
        exit(-1);
    }

    if(listen(sock, 50) < 0)    //start listening to it
    {
        perror("listen");
        exit(-1);
    }

    pthread_t* ptid = new pthread_t[poolSize];  //initialize the workerThread pthread_id container
    MyQueue* mq = new MyQueue(queueSize);       //and the queue

    for(int i = 0; i < poolSize; i++)   //start the worker threads
        pthread_create(&ptid[i], NULL, workerThread, (void*) mq);

    cout << endl;   //print the parameters
    cout << "Server's parameters:" << endl;
    cout << "\tPort: " << port << endl;
    cout << "\tThread Pool Size: " << poolSize << endl;
    cout << "\tQueue Size: " << queueSize << endl;

    while(true)
    {
        clientlen = sizeof(client);
        if((newsock = accept(sock, clientptr, &clientlen)) < 0) //whenever you get a client
        {
            perror("accept");
            exit(-1);
        }

        if((rem = gethostbyaddr((char*) &client.sin_addr.s_addr, sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL)
        {
            herror("gethostbyaddr");
            exit(-1);
        }

        printf("\nAccepted connection from %s\n", rem->h_name);

        pthread_t* pt = new pthread_t;  //start a new firstLineThread
        MyThreadArgs* mta = new MyThreadArgs(mq, newsock, sock, ptid, poolSize, pthread_self());
        pthread_create(pt, NULL, firstLineThread, (void*)mta);
        delete pt;
    }
}
