# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = p2nprobe
SOURCES = main.cpp # other_file.cpp Add your source files here
OBJECTS = $(SOURCES:.cpp=.o)

# Rules
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpcap

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
