#include "clientfunctions.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 7)   //check the input
        inputError();

    int ipFlag, portFlag, directoryFlag;

    for(ipFlag = 1; ipFlag < argc; ipFlag++)    //find the ip flag
        if(strcmp(argv[ipFlag], "-i") == 0)
            break;

    for(portFlag = 1; portFlag < argc; portFlag++)  //find the port flag
        if(strcmp(argv[portFlag], "-p") == 0)
            break;

    for(directoryFlag = 1; directoryFlag < argc; directoryFlag++)   //find the directory flag
        if(strcmp(argv[directoryFlag], "-d") == 0)
            break;

    if(ipFlag == 7 || portFlag == 7 || directoryFlag == 7)  //if any of them do not exist
        inputError();

    int port = getNumber(argv[portFlag + 1]);   //get the port

    cout << endl;   //print the parameters
    cout << "Client's parameters:" << endl;
    cout << "\tServer IP: " << argv[ipFlag + 1] << endl;
    cout << "\tPort: " << port << endl;
    cout << "\tDirectory Requested: " << argv[directoryFlag + 1] << endl << endl;

    int sock;

    struct sockaddr_in server;  //set the proper parameters
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    if((sock = socket(AF_INET, SOCK_STREAM, 0 )) < 0)   //set the socket
    {
        perror("socket");
        exit(-1);
    }

    if((rem = gethostbyname(argv[ipFlag + 1])) == NULL) //get the server properties
    {
        herror("gethostbyname");
        exit(-1);
    }

    server.sin_family = AF_INET;//and set them
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);

    if(connect(sock, serverptr, sizeof(server)) < 0)    //connect to the server
    {
        perror("connect");
        exit(-1);
    }

    cout << "Connecting to " << argv[ipFlag + 1] << ":" << port << endl << endl;

    sendName(sock, argv[directoryFlag + 1]);    //send your request

    getItems(sock); //and get your answer

    if(close(sock) == -1)   //close the socket
        perror("close");
}
