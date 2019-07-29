all:
	gcc complex_add.c -c -g -fPIC
	gcc -shared -o libcomplex_add.so complex_add.o
	gcc complex_sub.c -c -g -fPIC
	gcc -shared -o libcomplex_sub.so complex_sub.o
	gcc complex_mult.c -c -g -fPIC
	gcc -shared -o libcomplex_mult.so complex_mult.o
	gcc complex_div.c -c -g -fPIC
	gcc -shared -o libcomplex_div.so complex_div.o
	gcc main.c -c -g -DDEBUG
	gcc complex_print.c -c -g
	gcc main.o complex_print.c -o main -ldl
