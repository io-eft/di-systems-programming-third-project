#ifndef __MYTHREADS__HPP
#define __MYTHREADS__HPP
#include "mythreadargs.hpp"
#include "myinodelist.hpp"
#include "sendandreceive.hpp"
#include "serverfunctions.hpp"

void* firstLineThread(void* args);

void* workerThread(void* args);

void* terminator(void* args);

#endif
