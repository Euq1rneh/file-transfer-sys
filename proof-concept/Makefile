CC = gcc

CFLAGS = -Wall -g

TARGETS = server client

all: $(TARGETS)

server: server.c message.o
	$(CC) $(CFLAGS) -o server server.c message.o

client: client.c message.o
	$(CC) $(CFLAGS) -o client client.c message.o

message.o: message.c message.h
	$(CC) $(CFLAGS) -c message.c

clean:
	rm -f $(TARGETS) *.o
