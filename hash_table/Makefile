

PROG=hash

OBJS= main.o \
			hashtable.o

CFLAGS= -Wall -Wextra -pedantic -ggdb
CPPFLAGS= -DNDEBUG

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	 $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o
