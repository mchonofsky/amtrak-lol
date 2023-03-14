mymodel.so: main.o
	gcc -shared -O3 -o mymodel.so main.o -std=c99 
main.o: main.c
	gcc -c -O3 -o main.o main.c -fPIC -std=c99 
