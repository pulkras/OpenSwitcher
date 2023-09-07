CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = $(shell pkg-config --libs x11) $(shell pkg-config --libs xkbcommon)
INCLUDES = $(shell pkg-config --cflags x11) $(shell pkg-config --cflags xkbcommon)

SRCS = main1.c
OBJS = $(SRCS:.c=.o)
BIN = myproject

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BIN)