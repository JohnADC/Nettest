PROJECT1 = server
SOURCE1 = server.c wrappers.c

PROJECT2 = client
SOURCE2 = client.c wrappers.c

HEADERS = headers.h wrappers.h utility.h

CC = gcc
CFLAGS = -Wall -g
LDFLAGS =

OBJECTS1 = ${SOURCE1:.c=.o}
OBJECTS2 = ${SOURCE2:.c=.o}

all: $(PROJECT1) $(PROJECT2)

$(PROJECT1):$(OBJECTS1)
	$(CC) $(OBJECTS1) -o $(PROJECT1)

$(OBJECTS1):$(HEADERS)	 

$(PROJECT2):$(OBJECTS2)
	$(CC) $(OBJECTS2) -o $(PROJECT2)

$(OBJECTS2):$(HEADERS)

clean:
	rm -f $(PROJECT1) $(OBJECTS1) $(PROJECT2) $(OBJECTS2) *.core
