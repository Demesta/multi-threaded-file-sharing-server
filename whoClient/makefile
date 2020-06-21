OBJS 	= main.o sockets.o
SOURCE	= main.cpp sockets.cpp
HEADER  = sockets.h Arguments.h
OUT  	= whoClient
CC	= g++
FLAGS   = -c

$(OUT): $(OBJS)
	$(CC) -g -lpthread $(OBJS) -o $@

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp


sockets.o: sockets.cpp
	$(CC) $(FLAGS) sockets.cpp


run:
	./whoClient -q queries.txt -w 10 -sp 9003 -sip localhost

clean:
	rm -f $(OBJS) $(OUT)