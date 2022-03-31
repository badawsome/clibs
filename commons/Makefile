TARGET = commons.a

PWD = $(shell pwd)
SRCS  = $(shell find $(PWD)/src -type f -name *.c)
HEADERS = $(shell find $(PWD)/include -type f -name *.h)
INCLUDE = $(PWD)/include
INCLUDE_ARGS = $(addprefix -I,$(INCLUDE))

OBJS = $(SRCS:.c=.o)

#CC = clang
#CXX = clang++
CFLAGS = -O2 -g -Wall -Wextra -pthread $(INCLUDE_ARGS)
CPPFLAGS = -isystem $(GTEST_DIR)/include $(INCLUDE_ARGS)
CXXFLAGS = -std=c++17 -g -Wall -Wextra -pthread $(INCLUDE_ARGS)

$(OBJS):
	cd src && $(CC) $(CFLAGS) -c $(SRCS)
$(TARGET): $(OBJS) $(HEADERS)
	$(AR) $(ARFLAGS) $@ $^

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET) $(PWD)/$(TARGET)
