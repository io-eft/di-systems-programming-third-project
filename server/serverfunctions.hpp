#ifndef __SERVERFUNCTIONS__HPP
#define __SERVERFUNCTIONS__HPP
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "myinode.hpp"
#include "myinodelist.hpp"
#include "sendandreceive.hpp"
#include <fstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include "myqueue.hpp"
#include <signal.h>
#include <sstream>

void getDirectoryItems(const char* path, const char* pathPrefix, MyINodeList* myFiles, MyINodeList* myDirectories);

int sendFiles(int newsock);

//void sendDirectory(int sock, MyINode* mind);

char* concat(const char* a, const char* b);

char* concat(const char* a, const char* b, const char* c);

void sigHandler(int sig);

void inputError();

int getNumber(char* c);

#endif

