all: boundedBuffer.cpp mh.cpp airline.c testCode.sh
	g++ boundedBuffer.cpp -o boundedBuffer -lpthread
	g++ mh.cpp -o mh -lpthread
	gcc airline.c -o airline -lpthread
	chmod a+rx testCode.sh