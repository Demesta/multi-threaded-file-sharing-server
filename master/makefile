OBJS 	= AVLTree.o sockets.o Bucket.o countryHashTable.o diseaseHashTable.o Dir_list.o main.o PatientRecord.o utils.o HashTable.o signal_handler.o Dir_countries.o Records_list.o
SOURCE	= AVLTree.cpp sockets.cpp Bucket.cpp countryHashTable.cpp diseaseHashTable.cpp HashTable.cpp Dir_list.cpp main.cpp PatientRecord.cpp utils.cpp signal_handler.cpp Dir_countries.cpp Records_list.cpp
HEADER  = AVLTree.h Bucket.h sockets.h countryHashTable.h diseaseHashTable.h HashTable.h Dir_list.h PatientRecord.h utils.h TreeNode.h Arguments.h signal_handler.h Dir_countries.h Records_list.h 
OUT  	= master
CC	= g++
FLAGS   = -c

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

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



run:
	./master -w 4 -b 100 -s localhost -p 8008 -i input_dir

clean:
	rm -f $(OBJS) $(OUT)