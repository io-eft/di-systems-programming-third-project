#include "sendandreceive.hpp"

int readNumber(int sock)
{
    char buff[sizeof(int)]; //send a buffer the size of int
    int bitsRead = 0;   //a counter for the bits read
    int bitsLeft = sizeof(int); //bytes left to read
    int total = bitsLeft;   //total bytes to read

    while(bitsRead < total) //while there are still bytes to read
    {
        if(bitsRead < (bitsRead += read(sock, buff + bitsRead, total - bitsRead)))
        {   //keep reading and place the bytes with an offset to the correct place
            perror("readNumber");
            close(sock);
            exit(-1);
        }
    }
    return *((int*) buff);  //return it as an int
}

void readName(int sock, char* buff)
{   //same as above, but you read the numbers of bytes to read
    int bitsRead = 0;
    int total = readNumber(sock);//get the number of bytes to read
    int bitsLeft = total;
    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, &buff[bitsRead], total - bitsRead)))
        {
            perror("readName");
            close(sock);
            exit(-1);
        }
    }
}

mode_t readMode(int sock)
{   //same as the first, but with mode_t instead of int
    char buff[sizeof(sizeof(mode_t))];
    int bitsRead = 0;
    int bitsLeft = sizeof(mode_t);
    int total = bitsLeft;

    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, buff + bitsRead, total - bitsRead)))
        {
            perror("readMode");
            close(sock);
            exit(-1);
        }
    }
    return *((mode_t*) buff);
}

long readFileSize(int sock)
{   //same as the first, but with long instead of int
    char buff[sizeof(long)];
    int bitsRead = 0;
    int bitsLeft = sizeof(long);
    int total = bitsLeft;

    while(bitsRead < total)
    {
        if(bitsRead < (bitsRead += read(sock, buff + bitsRead, total - bitsRead)))
        {
            perror("readSize");
            close(sock);
            exit(-1);
        }
    }
    return *((long*) buff);
}

void sendNumber(int sock, int number)
{
    char* buff = (char*)&number;    //put a char pointer at the start of the number
    int sent, n;
    for(sent = 0; sent < sizeof(int); sent+=n)  //while you have not send all the bytes
    {
        if ((n = write(sock, buff+sent, sizeof(int)-sent)) == -1)   //keep sending, with an offset of the bytes sent already
        {
            perror("sendNumber");
            close(sock);
            exit(-1);
        }
    }
}

void sendName(int sock, const char* name)
{   //same as above, only send the length first
    sendNumber(sock, strlen(name));
    int sent, n;
    for(sent = 0; sent < strlen(name); sent+=n)
    {
        if ((n = write(sock, name+sent, strlen(name)-sent)) == -1)
        {
            perror("sendName");
            close(sock);
            exit(-1);
        }
    }
}

void sendFileSize(int sock, long fileSize)
{   //same as above
    char* buff = (char*)&fileSize;
    int sent, n;
    for(sent = 0; sent < sizeof(long); sent+=n)
    {
        if ((n = write(sock, buff+sent, sizeof(long)-sent)) == -1)
        {
            perror("sendFileSize");
            close(sock);
            exit(-1);
        }
    }
}

void sendMode(int sock, mode_t mode)
{   //same as above
    char* buff = (char*)&mode;
    int sent, n;
    for(sent = 0; sent < sizeof(mode_t); sent+=n)
    {
        if ((n = write(sock, buff+sent, sizeof(mode_t)-sent)) == -1)
        {
            perror("sendFileSize");
            close(sock);
            exit(-1);
        }
    }
}
