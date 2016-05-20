#include "myinode.hpp"

MyINode::MyINode(struct stat s, const char* n): iNodeNumber(s.st_ino), links(s.st_nlink), fileSize(s.st_size), lastModification(s.st_mtime), mode(s.st_mode), upToDate(false)
{
    if((name = (char*)calloc(strlen(n) + 1, sizeof(char))) == NULL)
    {
        perror("malloc in INode constructor");
        exit(-1);
    }
    strcpy(name, n);
}

MyINode::MyINode(long s, const char* n): iNodeNumber(s), links(0), fileSize(0), lastModification(0), mode(0), upToDate(false)
{
    //name = n;
    if((name = (char*)calloc(strlen(n) + 1, sizeof(char))) == NULL)
    {
        perror("malloc in INode constructor");
        exit(-1);
    }
    strcpy(name, n);
}

MyINode::~MyINode()
{
    if(name != NULL)
        free(name);
}

void MyINode::print()
{
    std::cout << "Name: " << name << std::endl;
  //  std::cout << "i-node Number: " << iNodeNumber << std::endl;
    //std::cout << "Links: " << links << std::endl;
    std::cout << "Size: " << fileSize << std::endl;
    //std::cout << "Last modification: " << lastModification << std::endl;
}

void MyINode::setUpToDate()
{
    upToDate = true;
}

char* MyINode::getName()
{
    /*char* n;
    if((n = malloc(strlen(name) + 1)) == NULL)
    {
        perror("malloc in getName");
        exit(-1);
    }
    strcpy(n, name);
    return n;*/
    return name;
}

long MyINode::getINodeNumber()
{
    return iNodeNumber;
}

long MyINode::getLinks()
{
    return links;
}

long MyINode::getFileSize()
{
    return fileSize;
}

long MyINode::getLastModification()
{
    return lastModification;
}

mode_t MyINode::getMode()
{
    return mode;
}

bool MyINode::isUpdated()
{
    return upToDate;
}
