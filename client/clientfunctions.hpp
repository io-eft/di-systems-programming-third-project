#ifndef __CLIENTFUNCTIONS__HPP
#define __CLIENTFUNCTIONS__HPP
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include "sendandreceive.hpp"
#include <sstream>
#include <errno.h>

void getItems(int sock);

void getFirstDirectory(int sock, int* counter);
void getDirectory(int sock, int* counter);
void getFile(int sock, int* counter);
void inputError();
int getNumber(char* c);

#endif

