g++ -std=c++11 -o proj_init.o -c proj_init.cpp
g++ -std=c++11 -o file1.o -c file1.cpp
g++ -std=c++11 -o file2.o -c file2.cpp
g++ -o multi_file proj_init.o file1.o file2.o
./multi_file
