

PROG=hash

OBJS= main.o \
			hashtable.o

CFLAGS= -Wall -Wextra -pedantic -ggdb -std=c99
# CPPFLAGS= -DNDEBUG
CC=cc

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

test: hashtable.o test.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	 $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o
