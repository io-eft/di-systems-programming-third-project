all: remoteClient clearo

remoteClient: remoteclient.o clientfunctions.o sendandreceive.o
	g++ -g3 -Wall remoteclient.o clientfunctions.o sendandreceive.o -o remoteClient

client.o: clienttest.cpp
	g++ -g3 -c clienttest.cpp

clientfunctions.o: clientfunctions.cpp
	g++ -g3 -c clientfunctions.cpp

sendandreceive.o: sendandreceive.cpp
	g++ -g3 -c sendandreceive.cpp

clearo: 
	rm -rf *.o

clean:
	rm -rf *.o remoteClient
