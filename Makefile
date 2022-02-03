CC=gcc
CXX=g++
CFLAGS += -Wall -Wextra -Wpedantic 
CPPFLAGS += -Wall -Wextra -Wpedantic 
CPPFLAGS += -std=c++2a
CPPFLAGS += -I/usr/include/CppUTest -include /usr/include/CppUTest/MemoryLeakDetectorNewMacros.h
LD_LIBRARIES = -L/usr/lib/x86_64-linux-gnu/ -lCppUTest -lCppUTestExt

SOURCE_FILES=\
	hash_table.c \
	lru.c \
	value.c

BUILD=build
OBJECT_FILES=$(SOURCE_FILES:%.c=$(BUILD)/%.o)

TEST_FILES=\
	all_tests.cpp \
	hash_table_test.cpp \
	lru_test.cpp

TEST_OBJECT_FILES=$(TEST_FILES:%.cpp=$(BUILD)/%.o)

all: $(BUILD)/tests

$(BUILD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $< -c

$(BUILD)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -o $@ $< -c

$(BUILD)/tests: $(TEST_OBJECT_FILES) $(OBJECT_FILES)
	g++ $^ -o $@ ${LD_LIBRARIES}

test:	$(BUILD)/tests
	build/tests
