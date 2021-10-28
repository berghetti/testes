
OBJS= main.o

PROG=hash
CC=gcc

CFLAGS= -Wall -Wextra -pedantic
CPPFLAGS= -DNDEBUG

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	- ./$(PROG)
	rm -f ./*.o

%.o: %.c
	 $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
