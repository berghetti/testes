
CFLAGS = -Wall -Wextra -O2 -ggdb

OBJS = vector.o \
			 main.o

vector: $(OBJS)
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm *.o
