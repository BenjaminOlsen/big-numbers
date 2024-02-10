# Compiler settings - Can change based on your needs
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Project files
SOURCES = bigNumbers.cpp bigInt.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bigNumbers

# The first rule is the one executed when no parameters are fed into the makefile
all: $(EXECUTABLE)

# Linking the executable from the object files
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets are not files
.PHONY: all clean

