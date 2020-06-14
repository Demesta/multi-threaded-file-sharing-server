cmake_minimum_required(VERSION 3.15)
project(syspro2)

set(CMAKE_CXX_STANDARD 14)

add_executable(syspro2 main.cpp Arguments.h Dir_list.cpp Dir_list.h Dir_countries.cpp Dir_countries.h Records_list.cpp Records_list.h PatientRecord.cpp PatientRecord.h utils.h utils.cpp diseaseHashTable.cpp diseaseHashTable.h HashTable.cpp HashTable.h AVLTree.cpp AVLTree.h TreeNode.h Bucket.cpp Bucket.h countryHashTable.cpp countryHashTable.h signal_handler.cpp signal_handler.h sockets.cpp sockets.h)