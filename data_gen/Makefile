TARGET = data_gen.a

PWD = $(shell pwd)
SRCS  = $(shell find $(PWD)/src -type f -name *.c)
HEADERS = $(shell find $(PWD)/include -type f -name *.h)
INCLUDE = $(PWD)/include

OBJS = $(SRCS:.c=.o)

#CC = clang
#CXX = clang++
CFLAGS = -O2 -g -Wall -Wextra -pthread -I $(INCLUDE)
CPPFLAGS = -isystem $(GTEST_DIR)/include -I $(INCLUDE)
CXXFLAGS = -std=c++17 -g -Wall -Wextra -pthread -I $(INCLUDE)

dbg.o:
	$(CC) $(CFLAGS) -o $(PWD)/src/$@ -c $(PWD)/src/dbg.c
avl.o:
	$(CC) $(CFLAGS) -o $(PWD)/src/$@ -c $(PWD)/src/avl.c
$(TARGET): $(OBJS) $(HEADERS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(PWD)/$(TARGET)