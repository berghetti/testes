gcc -c -o s.o swap.S -Wall -ggdb -O2 && gcc -c -o t.o -Wall -O0 thread2.c -ggdb -O2 -Wall && gcc -o t t.o s.o && ./t
