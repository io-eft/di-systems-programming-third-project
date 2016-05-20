#include "clientfunctions.hpp"

void getItems(int sock)
{
    int directories = readNumber(sock); //read the number of directories to be received
    int files = readNumber(sock);   //read the number of files to be received
    int preExistingDirectories = 0;
    int removedFiles = 0;
    //inform the user
    std::cout << "There are " << directories << " directories containing " << files << " files" << std::endl << std::endl;

    if(directories > 0) //if there are directories to be received
        getFirstDirectory(sock, &preExistingDirectories);    //get the first one

    for(int i = 1; i < directories; i++)    //then get the remaining
        getDirectory(sock, &preExistingDirectories);

    for(int i = 0; i < files; i++)  //receive the files one by one
        getFile(sock, &removedFiles);

    std::cout << std::endl << "Received " << directories << " directories";
    if(preExistingDirectories != 0)
        std::cout << " (" << preExistingDirectories << " already existed)";
    std::cout << " and " << files << " files";
    if(removedFiles != 0)
        std::cout << " (" << removedFiles << " already existed and were replaced)";
    std::cout << "." << std::endl << std::endl;
}

void getFirstDirectory(int sock, int* counter)
{
    char buff[PATH_MAX + 1];
    memset(buff, '\0', PATH_MAX + 1);

    readName(sock, buff);   //read the name of the directory
    mode_t directoryMode = readMode(sock);  //and the mode

    std::string s = buff;
    for(int i = 0; i < s.length(); i++) //since it's the first one, check to see if it's a subdirectory
    {
        if(s[i] == '/')
            if(mkdir(s.substr(0, i).c_str(), 0777) < 0) //create all directories appearing in the path
                if(errno != EEXIST)
                    perror("mkdir");
    }
    if(mkdir(buff, directoryMode) < 0)  //and the directory itself
        if(errno != EEXIST)
            perror("mkdir");
        else
            (*counter)++;
}

void getDirectory(int sock, int* counter)
{
    char buff[PATH_MAX + 1];
    memset(buff, '\0', PATH_MAX + 1);

    readName(sock, buff);   //read the name of the directory
    mode_t directoryMode = readMode(sock);  //and it's mode

    if(mkdir(buff, directoryMode) < 0)  //create the directory
        if(errno != EEXIST)
            perror("mkdir");
        else
            (*counter)++;
}

void getFile(int sock, int* counter)
{
    long pagesize = sysconf(_SC_PAGESIZE);  //get the page size
    char* buff = (char*)malloc(pagesize);   //allocate a buffer with it
    memset(buff, '\0', pagesize);   //initialize it

    readName(sock, buff);   //read the name of the file
    mode_t mode = readMode(sock);   //and it's mode
    long fileSize = readFileSize(sock); //it's size
    int fd;

    if((fd = open(buff, O_RDONLY)) > 0) //if you can open it, meaning it exists
    {
        if(close(fd) == -1)//close it
            perror("close");
        if(remove(buff)== -1)   //and delete it
            perror("remove");
        (*counter)++;
    }

    if((fd = open(buff, O_CREAT | O_RDWR, mode)) < 0)   //create the file
    {
        perror("open in getFile");
        if(close(sock) == -1)
            perror("close");
        exit(-1);
    }

    long totalBitsRead = 0, totalBitsWritten = 0;   //set counters
    int bitsRead = 0, bitsWritten = 0, n = 0;
    std::cout << "Receiving " << buff << std::endl; //let the user know which file you are receiving
    while(totalBitsRead < fileSize) //while there are still bytes to read
    {
        int bits = pagesize;
        if(fileSize - totalBitsRead < pagesize)     //check if the pagesize or remaining bytes is smaller
            bits = fileSize - totalBitsRead;

        if((bitsRead = read(sock, buff, bits)) < 0) //try to read that number of bytes from socket
        {
            perror("read - getFile");
            if(close(sock) == -1)
                perror("close");
            if(close(fd) == -1)
                perror("close");
            exit(-1);
        }
        totalBitsRead += bitsRead;  //increase the counter of bytes readen

        for(bitsWritten = 0; bitsWritten < bitsRead; bitsWritten += n)  //try to write all the bytes read by the socket to the new file
        {
            if ((n = write(fd, buff + bitsWritten, bitsRead - bitsWritten)) == -1)
            {
                perror("write - getFile");
                if(close(sock) == -1)
                    perror("close");
                if(close(fd) == -1)
                    perror("close");
                exit(-1);
            }
        }
        totalBitsWritten += bitsWritten;    //increase the counter of all bites written
      //  std::cout << totalBitsRead << " out of " << fileSize << std::endl;
    }
    if(fileSize != totalBitsRead || fileSize != totalBitsWritten)   //in case the numnbers don't match, let the user know
        std::cerr << "File not transfered properly" << std::endl;
    sendFileSize(sock, totalBitsRead);      //send the bitsread to server for comfirmation
    free(buff);
    if(close(fd) == -1)
        perror("close");
}

void inputError()
{   //inform the user about proper arguments and exit
    std::cout << "Please use the correct arguements" << std::endl;
    std::cout << "-i <server_ip>" << std::endl;
    std::cout << "-p <server_port>" << std::endl;
    std::cout << "-d <directory>" << std::endl;
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
