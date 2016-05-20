#include "sendandreceive.hpp"

int readNumber(int sock)    //most of these functions are the same as the client ones
{
    char buff[sizeof(int)];
    int bitsRead = 0;
    int bitsLeft = sizeof(int);
    int total = bitsLeft;

    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, buff + bitsRead, total - bitsRead)))
        {
            perror("readNumber");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
    return *((int*) buff);
}

void readName(int sock, char* buff)
{
    int bitsRead = 0;
    int total = readNumber(sock);
    int bitsLeft = total;
    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, &buff[bitsRead], total - bitsRead)))
        {
            perror("readName");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
}

mode_t readMode(int sock)
{
    char buff[sizeof(sizeof(mode_t))];
    int bitsRead = 0;
    int bitsLeft = sizeof(mode_t);
    int total = bitsLeft;

    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, buff + bitsRead, total - bitsRead)))
        {
            perror("readMode");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
    return *((mode_t*) buff);
}

long readFileSize(int sock)
{
    char buff[sizeof(long)];
    int bitsRead = 0;
    int bitsLeft = sizeof(long);
    int total = bitsLeft;

    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, buff + bitsRead, total - bitsRead)))
        {
            perror("readSize");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
    return *((long*) buff);
}

void sendNumber(int sock, int number)
{
    char* buff = (char*)&number;
    int sent, n;
    for(sent = 0; sent < sizeof(int); sent+=n)
    {
        if ((n = write(sock, buff+sent, sizeof(int)-sent)) == -1)
        {
            perror("sendNumber");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
}

void sendName(int sock, const char* name)
{
    sendNumber(sock, strlen(name));
    int sent, n;
    for(sent = 0; sent < strlen(name); sent+=n)
    {
        if ((n = write(sock, name+sent, strlen(name)-sent)) == -1)
        {
            perror("sendName");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
}

void sendFileSize(int sock, long fileSize)
{
    char* buff = (char*)&fileSize;
    int sent, n;
    for(sent = 0; sent < sizeof(long); sent+=n)
    {
        if ((n = write(sock, buff+sent, sizeof(long)-sent)) == -1)
        {
            perror("sendFileSize");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
}

void sendMode(int sock, mode_t mode)
{
    char* buff = (char*)&mode;
    int sent, n;
    for(sent = 0; sent < sizeof(mode_t); sent+=n)
    {
        if ((n = write(sock, buff+sent, sizeof(mode_t)-sent)) == -1)
        {
            perror("sendFileSize");
            if(close(sock) == -1)
                perror("close");
            exit(-1);
        }
    }
}

void sendFile(int sock, MyINode* mind)
{
    sendName(sock, mind->getName());        //send the name
    sendMode(sock, mind->getMode());        //send the mode
    sendFileSize(sock, mind->getFileSize());    //send the size

    int fd;
    if((fd = open(mind->getName(), O_RDONLY)) < 0)  //open it
    {
        perror("open in sendFile");
        if(close(sock) == -1)
                perror("close");
        exit(-1);
    }

    long messageLength = sysconf(_SC_PAGESIZE); //get the size of the buffer
    char* buff = (char*)malloc(messageLength);  //allocate memory for it
    int bitsRead, bitsWritten, sent;

    while((bitsRead = read(fd, buff, messageLength)) > 0)   //while there are still bytes to read
    {
        for( sent = 0; sent < bitsRead; sent += bitsWritten)    //send everything you'vr read
        {
            if ((bitsWritten = write(sock, buff+sent, bitsRead - sent)) == -1)
            {
                perror("sendFile - write");
                free(buff);
                if(close(sock) == -1)
                    perror("close");
                if(close(fd) == -1)
                    perror("close");
                exit(-1);
            }
        }
    }

    if(bitsRead == -1)  //if there was an error in read
    {
        perror("sendFile - read");
        if(close(sock) == -1)
            perror("close");
        if(close(fd) == -1)
            perror("close");
        free(buff);
        exit(-1);
    }
    if(readFileSize(sock) != mind->getFileSize())   //get the repsonse, and let the user know if there was an error
        std::cerr << "File not transfered properly" << std::endl;
    free(buff);
    if(close(fd) == -1)
        perror("close");
}

void sendDirectory(int sock, MyINode* mind)
{
    sendName(sock, mind->getName());    //send the name
    sendMode(sock, mind->getMode());    //send the mode
}
