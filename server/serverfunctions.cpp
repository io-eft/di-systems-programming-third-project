#include "serverfunctions.hpp"

void getDirectoryItems(const char* path, const char* pathPrefix, MyINodeList* myFiles, MyINodeList* myDirectories)
{   //get items of a given directory
    struct stat s;
    struct dirent* direntptr;
    struct dirent* result;
    DIR* dirptr;

    char* sourceDirectory = concat(pathPrefix, path);   //get the path of the directory
    if(stat(sourceDirectory, &s) == -1) //stat it
        perror("stat");
    else
    {
        MyINode* mind = new MyINode(s, path);   //create a myinode
        myDirectories->insertEntry(mind);       //and add it to the list of directories
    }
    if((direntptr = (dirent*)malloc(offsetof(struct dirent, d_name) + sysconf(_PC_NAME_MAX) + 1)) == NULL)
        return; //allocate memory for direntptr - code taken from an example online
    if((dirptr = opendir(sourceDirectory)) == NULL) //open the directory
        perror("opendir");
    else
    {
        while(true)
        {   //keep reading the directory until the end
            readdir_r(dirptr, direntptr, &result);
            if(result == NULL)
                break;
            if(strcmp(direntptr->d_name, ".") != 0 && strcmp(direntptr->d_name, ".."))
            {   //ignore current and parent files
                char* relativePath = concat(path, "/", direntptr->d_name);  //create the relative path
                char* nam = concat(pathPrefix, relativePath);   //and the absolute path
                if(stat(nam, &s) == -1) //stat them
                    perror("stat");
                else
                {
                    if(S_ISREG(s.st_mode))  //if those are files
                    {   //create a new myinode
                        MyINode* mind = new MyINode(s, relativePath);
                        myFiles->insertEntry(mind); //and add it to the list
                    }
                    if(S_ISDIR(s.st_mode))  //if it's a directory, run it recursively
                        getDirectoryItems(relativePath, pathPrefix, myFiles, myDirectories);
                }
                free(relativePath); //free the memory
                free(nam);
            }
        }
    }
    if(closedir(dirptr) == -1)  //close the dir
        perror("closedir");
    free(direntptr);    //free the memory
    free(sourceDirectory);  //free the memory
}

char* concat(const char* a, const char* b)
{
    char* result;   //get a pointer and allocate memory for it
    if((result = (char*)calloc(strlen(a) + strlen(b) + 1, sizeof(char))) == NULL)
    {
        perror("malloc in concat");
        exit(-1);
    }
    strcat(result, a);  //add the first string to it
    strcat(result, b);  //and the second
    return result;  //return it
}

char* concat(const char* a, const char* b, const char* c)
{   //same as above, with 3 arguments
    char* result;
    if((result = (char*)calloc(strlen(a) + strlen(b) + strlen(c) + 1, sizeof(char))) == NULL)
    {
        perror("calloc in concat");
        exit(-1);
    }
    strcat(result, a);
    strcat(result, b);
    strcat(result, c);
    return result;
}

void sigHandler(int sig)
{   //signal handler, end the thread if it's received
    if(sig == SIGRTMIN+1)
        pthread_exit(NULL);
}

void inputError()
{
    std::cout << "Please use the correct arguements" << std::endl;
    std::cout << "-p <port>" << std::endl;
    std::cout << "-s <thread_pool_size>" << std::endl;
    std::cout << "-q <queue_size>" << std::endl;
    exit(0);
}

int getNumber(char* c)
{   //get number from a c_string using stringstream
    int number;
    std::stringstream ss(c);
    if(!(ss >> number))
        inputError();
    return number;
}
/*
void getDirectoryItems(const char* path, const char* pathPrefix, MyINodeList* myFiles, MyINodeList* myDirectories)
{   //get items of a given directory
    struct stat s;
    struct dirent* direntptr;
    //struct dirent* result;
    DIR* dirptr;

    char* sourceDirectory = concat(pathPrefix, path);   //get the path of the directory
    if(stat(sourceDirectory, &s) == -1) //stat it
        perror("stat");
    else
    {
        MyINode* mind = new MyINode(s, path);   //create a myinode
        myDirectories->insertEntry(mind);       //and add it to the list of directories
    }
  //  if((direntptr = (dirent*)malloc(offsetof(struct dirent, d_name) + sysconf(_PC_NAME_MAX) + 1)) == NULL)
    //    return; //allocate memory for direntptr - code taken from an example online
    if((dirptr = opendir(sourceDirectory)) == NULL) //open the directory
        perror("opendir");
    else
    {
        while((direntptr = readdir(dirptr)) != NULL)
        {   //keep reading the directory until the end
           // readdir_r(dirptr, direntptr, &result);
           // if(result == NULL)
             //   break;
            if(strcmp(direntptr->d_name, ".") != 0 && strcmp(direntptr->d_name, ".."))
            {   //ignore current and parent files
                char* relativePath = concat(path, "/", direntptr->d_name);  //create the relative path
                char* nam = concat(pathPrefix, relativePath);   //and the absolute path
                if(stat(nam, &s) == -1) //stat them
                    perror("stat");
                else
                {
                    if(S_ISREG(s.st_mode))  //if those are files
                    {   //create a new myinode
                        MyINode* mind = new MyINode(s, relativePath);
                        myFiles->insertEntry(mind); //and add it to the list
                    }
                    if(S_ISDIR(s.st_mode))  //if it's a directory, run it recursively
                        getDirectoryItems(relativePath, pathPrefix, myFiles, myDirectories);
                }
                free(relativePath); //free the memory
                free(nam);
            }
        }
    }
    if(closedir(dirptr) == -1)  //close the dir
        perror("closedir");
    //free(direntptr);    //free the memory
    free(sourceDirectory);  //free the memory
}*/
