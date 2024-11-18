# ISA project - PCAP NetFlow v5 exporter
# Author: Michálek Kryštof (xmicha94) 

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = p2nprobe
SOURCES = main.cpp packet.cpp flow.cpp netflowV5.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lpcap

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
