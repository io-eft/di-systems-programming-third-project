all: dataServer clearo

dataServer: dataserver.o serverfunctions.o myinode.o myinodelist.o sendandreceive.o myjob.o myqueue.o mythreads.o mythreadargs.o
	g++ -g3 -Wall dataserver.o serverfunctions.o myinode.o myinodelist.o sendandreceive.o myjob.o myqueue.o mythreads.o mythreadargs.o -o dataServer -lpthread

dataserver.o: dataserver.cpp
	g++ -g3 -c dataserver.cpp

myinodelist.o: myinodelist.cpp
	g++ -c -g3 myinodelist.cpp

myinode.o: myinode.cpp
	g++ -c -g3 myinode.cpp

serverfunctions.o: serverfunctions.cpp
	g++ -c -g3 serverfunctions.cpp

sendandreceive.o: sendandreceive.cpp
	g++ -c -g3 sendandreceive.cpp

myqueue.o: myqueue.cpp
	g++ -c -g3 myqueue.cpp

myjob.o: myjob.cpp
	g++ -c -g3 myjob.cpp

mythreads.o: mythreads.cpp
	g++ -c -g3 mythreads.cpp

mythreadargs.o: mythreadargs.cpp
	g++ -c -g3 mythreadargs.cpp

clearo:
	rm -rf *.o
clean:
	rm -rf *.o dataServer
