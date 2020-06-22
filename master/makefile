OBJS 	= AVLTree.o sockets.o Bucket.o countryHashTable.o diseaseHashTable.o Dir_list.o main.o PatientRecord.o utils.o HashTable.o signal_handler.o Dir_countries.o Records_list.o logging.o
SOURCE	= AVLTree.cpp sockets.cpp Bucket.cpp countryHashTable.cpp diseaseHashTable.cpp HashTable.cpp Dir_list.cpp main.cpp PatientRecord.cpp utils.cpp signal_handler.cpp Dir_countries.cpp Records_list.cpp logging.cpp
HEADER  = AVLTree.h Bucket.h sockets.h countryHashTable.h diseaseHashTable.h HashTable.h Dir_list.h PatientRecord.h utils.h TreeNode.h Arguments.h signal_handler.h Dir_countries.h Records_list.h logging.h
OUT  	= master
CC	= g++
FLAGS   = -c -std=c++11

$(OUT): $(OBJS)
	$(CC) -g -std=c++11 $(OBJS) -o $@

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

AVLTree.o: AVLTree.cpp
	$(CC) $(FLAGS) AVLTree.cpp

Bucket.o: Bucket.cpp
	$(CC) $(FLAGS) Bucket.cpp

countryHashTable.o: countryHashTable.cpp
	$(CC) $(FLAGS) countryHashTable.cpp

diseaseHashTable.o: diseaseHashTable.cpp
	$(CC) $(FLAGS) diseaseHashTable.cpp

HashTable.o: HashTable.cpp
	$(CC) $(FLAGS) HashTable.cpp

Dir_list.o: Dir_list.cpp
	$(CC) $(FLAGS) Dir_list.cpp

PatientRecord.o: PatientRecord.cpp
	$(CC) $(FLAGS) PatientRecord.cpp

Dir_countries.o: Dir_countries.cpp
	$(CC) $(FLAGS) Dir_countries.cpp

signal_handler.o: signal_handler.cpp
	$(CC) $(FLAGS) signal_handler.cpp

Records_list.o: Records_list.cpp
	$(CC) $(FLAGS) Records_list.cpp

utils.o: utils.cpp
	$(CC) $(FLAGS) utils.cpp

sockets.o: sockets.cpp
	$(CC) $(FLAGS) sockets.cpp

logging.o: logging.cpp
	$(CC) $(FLAGS) logging.cpp


run:
	./master -w 4 -b 100 -s localhost -p 9002 -i input_dir

clean:
	rm -f $(OBJS) $(OUT)
