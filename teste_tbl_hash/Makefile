
OBJS= main.o \


PROG=hash

CFLAGS= -Wall -Wextra -pedantic
# CPPFLAGS= -DNDEBUG

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	 $(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f *.o
