
# Compiler
CXX := g++

# Compiler Flags 
CXXFLAGS := -std=c++23 -Wall -Wextra

# Linker Flags
LDFLAGS := 

# executable name

TARGET := main

# Source Files
SRC := main.cpp 

# Object files
OBJ := $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile
main.o: main.cpp Math/Matrix.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)




