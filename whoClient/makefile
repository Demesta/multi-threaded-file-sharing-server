OBJS 	= main.o sockets.o logging.o
SOURCE	= main.cpp sockets.cpp logging.cpp
HEADER  = sockets.h Arguments.h logging.h
OUT  	= whoClient
CC	= g++
FLAGS   = -c -pthread -std=c++11

$(OUT): $(OBJS)
	$(CC) -g -std=c++11 -pthread $(OBJS) -o $@

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp


sockets.o: sockets.cpp
	$(CC) $(FLAGS) sockets.cpp

logging.o: logging.cpp
	$(CC) $(FLAGS) logging.cpp


run:
	./whoClient -q queries.txt -w 2 -sp 9003 -sip localhost

clean:
	rm -f $(OBJS) $(OUT)
