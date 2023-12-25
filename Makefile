CXX      = g++
LD       = g++
CXXFLAGS = -g -std=c++17 -Wall -pedantic
OUTPUT   = wam

SOURCES = $(wildcard $(shell find . -name '*.cpp'))
OBJS    = $(SOURCES:.cpp=.o)


all: $(OUTPUT)

compile: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(LD) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o */*.o $(OUTPUT)
 
run: $(OUTPUT)
	./$(OUTPUT)