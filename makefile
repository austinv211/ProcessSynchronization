all: boundedBuffer.cpp mh.cpp airline.c
	g++ boundedBuffer.cpp -o boundedBuffer -lpthread
	g++ mh.cpp -o mh -lpthread
	gcc airline.c -o airline -lpthread