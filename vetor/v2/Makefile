
CFLAGS = -Wall -Wextra -pedantic -ggdb -O2

OBJS = vector.o \
			 main.o

vector: $(OBJS)
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

clean:
	rm *.o
