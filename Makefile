CXX      = g++
LD       = g++
CXXFLAGS = -std=c++17 -Wall -pedantic

OUTPUT      = wam
TEST_OUTPUT = wam_test

SOURCES = $(wildcard $(shell find . -name '*.cpp'))
OBJS    = $(SOURCES:.cpp=.o)

all: $(OUTPUT) $(TEST_OUTPUT)

compile: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(LD) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(wildcard $(shell find . -name '*.o')) $(OUTPUT) $(TEST_OUTPUT)
 
run: $(OUTPUT)
	./$(OUTPUT) --no-run

test: $(OUTPUT)
	./$(OUTPUT) --exit