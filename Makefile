# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 

# Target executable name
TARGET = gilmore

# Source files
SRCS = main.cpp gilmore.cpp first_order_logic.cpp herbrand.cpp

# Header files
HEADERS = gilmore.h first_order_logic.h interpretation.h herbrand.h

# Default target - build the executable
all: $(TARGET)

# Build the executable
$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up
clean:
	rm -f $(TARGET) *.o

# Phony targets
.PHONY: all clean
