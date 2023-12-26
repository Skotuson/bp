CXX      = g++
LD       = g++
CXXFLAGS = -g -std=c++17 -Wall -pedantic

OUTPUT      = wam
TEST_OUTPUT = wam_test

SOURCES = $(wildcard $(shell find ./src/ -name '*.cpp'))
OBJS    = $(SOURCES:.cpp=.o)

TEST_SOURCES = $(wildcard $(shell find ./test/ -name '*.cpp'))
TEST_OBJS    = $(TEST_SOURCES:.cpp=.o)


all: $(OUTPUT)

compile: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(LD) $(CXXFLAGS) -o $@ $^

$(TEST_OUTPUT): $(TEST_OBJS)
	$(LD) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(wildcard $(shell find . -name '*.o')) $(OUTPUT)
 
run: $(OUTPUT)
	./$(OUTPUT)

test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)