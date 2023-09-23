CC = gcc
CFLAGS = -Wall -g 

LDFLAGS += $(shell pkg-config --libs x11)
CFLAGS  += $(shell pkg-config --cflags x11)

LDFLAGS += $(shell pkg-config --libs xtst)
CFLAGS  += $(shell pkg-config --cflags xtst)

LDFLAGS += $(shell pkg-config --libs xkbcommon)
CFLAGS  += $(shell pkg-config --cflags xkbcommon)

LDFLAGS += $(shell pkg-config --libs icu-uc icu-io)
CFLAGS  += $(shell pkg-config --cflags icu-uc icu-io)

LDFLAGS += $(shell pkg-config --libs libinput)
CFLAGS  += $(shell pkg-config --cflags libinput)

LDFLAGS += $(shell pkg-config --libs libudev)
CFLAGS  += $(shell pkg-config --cflags libudev)

LDFLAGS += $(shell pkg-config --libs libevdev)
CFLAGS  += $(shell pkg-config --cflags libevdev)


SRCS = main.c
OBJS = $(SRCS:.c=.o)
BIN = openswitcher
PROGRAMPATH = /usr/bin/

.PHONY: all clean install uninstall

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BIN)

install:
	bash ./install.sh

uninstall:
	sudo $(RM)  $(PROGRAMPATH)$(BIN)