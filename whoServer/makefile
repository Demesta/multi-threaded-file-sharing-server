OBJS 	= main.o utils.o Hash.o List.o
SOURCE	= main.cpp utils.cpp Hash.cpp List.cpp
HEADER  = utils.h Hash.h Worker_list.h List.h Arguments.h
OUT  	= whoServer
CC	= g++
FLAGS   = -c

$(OUT): $(OBJS)
	$(CC) -g -lpthread $(OBJS) -o $@

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Hash.o: Hash.cpp
	$(CC) $(FLAGS) Hash.cpp


List.o: List.cpp
	$(CC) $(FLAGS) List.cpp


utils.o: utils.cpp
	$(CC) $(FLAGS) utils.cpp


run:
	./whoServer -q 9003 -s 8008 -w 4 -b 100

clean:
	rm -f $(OBJS) $(OUT)