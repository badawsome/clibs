TARGET = avl

PWD = $(shell pwd)
COMMONS_DIR = $(PWD)/../commons
SRCS  = $(shell find $(PWD)/src -type f -name *.c)
HEADERS = $(shell find $(PWD)/include -type f -name *.h)
INCLUDE = $(PWD)/include $(PWD)/../commons/include
INCLUDE_ARGS = $(addprefix -I,$(INCLUDE))

#OBJS = $(SRCS:.c=.o)

#CC = clang
#CXX = clang++
CFLAGS = -O2 -g -Wall -Wextra -pthread $(INCLUDE_ARGS)
CPPFLAGS = -isystem $(GTEST_DIR)/include $(INCLUDE_ARGS)
CXXFLAGS = -std=c++17 -g -Wall -Wextra -pthread $(INCLUDE_ARGS)

$(TARGET).o:
	$(CC) $(CFLAGS) -o $(PWD)/$@ -c $(PWD)/src/$(TARGET).c
$(TARGET).a: $(TARGET).o $(HEADERS) $(COMMONS_DIR)/commons.a
	$(AR) $(ARFLAGS) $@ $^

.PHONY: all clean

all: $(TARGET).a

clean:
	$(RM) $(TARGET).o $(TARGET).a $(PWD)/$(TARGET)
