

# PROG=hash

CFLAGS= -Wall -Wextra -pedantic -ggdb -std=c99
# CPPFLAGS= -DNDEBUG
CC=gcc

# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $^ -o $@

# implicity rules
ht2array: ht2array.o hashtable.o

test: test.o hashtable.o

jhash: jhash.o hashtable.o

%.o: %.c
	 $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o
