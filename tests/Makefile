# unit test
TEST_TARGET = main_test

PWD = $(shell pwd)
TEST_DIR = $(PWD)
TEST_SRCS = $(shell find $(PWD) -type f -depth 1 -name *.cc)
LIBS = avl

LIBS_INCLUDE = $(addsuffix /include, $(addprefix $(PWD)/../, $(LIBS)))
LIBS_INCLUDE_ARGS = $(addprefix -I, $(LIBS_INCLUDE))
LINK_LIBS = $(addsuffix .a, $(join $(addsuffix /,$(addprefix $(PWD)/../, $(LIBS))), $(LIBS)))
TESTS_SOURCES = $(wildcard *.cc)
TEST_OBJS = $(addprefix $(PWD)/,$(TESTS_SOURCES:.cc=.o))

#CC = clang
#CXX = clang++
CFLAGS = -O2 -g -Wall -Wextra -pthread $(LIBS_INCLUDE_ARGS)
CPPFLAGS = -isystem $(GTEST_DIR)/include $(LIBS_INCLUDE_ARGS)
CXXFLAGS = -std=c++17 -g -Wall -Wextra -pthread $(LIBS_INCLUDE_ARGS)

GTEST_DIR = $(TEST_DIR)/googletest/googletest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $(TEST_DIR)/$@
gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $(TEST_DIR)/$@
gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^
gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(TEST_TARGET): $(TEST_OBJS) gtest_main.a $(LINK_LIBS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)
clean:
	$(RM) $(TEST_DIR)/$(TEST_TARGET) $(TEST_DIR)/$(TEST_TARGET).o $(TEST_OBJS) \
	$(TEST_DIR)/gtest-all.o $(TEST_DIR)/gtest_main.a $(TEST_DIR)/gtest_main.o
