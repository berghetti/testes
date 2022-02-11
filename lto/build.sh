# with option -flto the compiler can put inline functions same in others modules

gcc -c main.c -o main.o -Wall -Wextra -O2 -flto
gcc -c sum.c -o sum.o -Wall -Wextra -O2 -flto

gcc -o sum main.o sum.o -Wall -Wextra -O2 -flto
