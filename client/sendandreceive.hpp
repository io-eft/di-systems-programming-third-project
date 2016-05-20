#ifndef __SENDANDRECEIVE__HPP
#define __SENDANDRECEIVE__HPP
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int readNumber(int sock);
void readName(int sock, char* buff);
mode_t readMode(int sock);
long readFileSize(int sock);

void sendNumber(int sock, int number);
void sendName(int sock, const  char* name);
void sendFileSize(int sock, long fileSize);
void sendMode(int sock, mode_t mode);

#endif
