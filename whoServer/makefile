OBJS 	= main.o utils.o Hash.o List.o sockets.o logging.o
SOURCE	= main.cpp utils.cpp Hash.cpp List.cpp sockets.cpp logging.cpp
HEADER  = utils.h Hash.h Worker_list.h List.h Arguments.h sockets.h logging.h
OUT  	= whoServer
CC	= g++
FLAGS   = -c -std=c++11 -pthread

$(OUT): $(OBJS)
	$(CC) -g -std=c++11 -pthread $(OBJS) -o $@

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Hash.o: Hash.cpp
	$(CC) $(FLAGS) Hash.cpp


List.o: List.cpp
	$(CC) $(FLAGS) List.cpp


utils.o: utils.cpp
	$(CC) $(FLAGS) utils.cpp

sockets.o: sockets.cpp
	$(CC) $(FLAGS) sockets.cpp

logging.o: logging.cpp
	$(CC) $(FLAGS) logging.cpp


run:
	./whoServer -q 9003 -s 9002 -w 10 -b 100

clean:
	rm -f $(OBJS) $(OUT)
