
gcc -fPIC -shared -O0 -o load.so load.c
gcc -o tls main.c -ggdb -mfsgsbase -ldl
LD_PRELOAD=./load.so ./tls
