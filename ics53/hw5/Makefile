CC := gcc

LIB := $(shell find lib -type f -name *.o)
SRC := $(shell find src -not -path '*/\.*' -type f -name *.c)
EXSRC := $(shell find iterative -not -path '*/\.*' -type f -name *.c)
INC := -I include

DFLAGS := -g -DDEBUG -DCOLOR
CFLAGS := $(INC) $(DFLAGS)


.PHONY: clean all setup iterative

all: server iterative

server: setup
	$(CC) $(CFLAGS) $(LIB) $(SRC) -o bin/server -lpthread

iterative: setup
	$(CC) $(CFLAGS) $(LIB) $(EXSRC) -o bin/iterative -lpthread


setup:
	mkdir -p bin

clean:
	$(RM) -r bin
