TARGET = avl
PWD = $(shell pwd)
SRCS  = $(shell find ./src     -type f -name *.c)
HEADERS = $(shell find ./include -type f -name *.h)
INCLUDE = $(PWD)/include
OBJS = $(SRCS:.c=.o)

CC = clang
CFLAGS = -O2 -Wall -I $(INCLUDE)

$(TARGET): $(OBJS) $(HEADS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)


.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
