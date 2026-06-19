
# Compiler
CXX := g++

# Compiler Flags 
CXXFLAGS := -std=c++23 -Wall -Wextra

# 
INCLUDES := -IMath -Iexternal/doctest

# Linker Flags
LDFLAGS := 


# Main Application
# -----------------------
# executable name
TARGET := main
# Source Files
SRC := main.cpp 
# Object files
OBJ := $(SRC:.cpp=.o)
# -----------------------


# Tests 
# -----------------------
# executable name
TEST_TARGET := tests
# Source Files
TEST_SRC := Test/test_matrix.cpp
# Object files
TEST_OBJ := $(TEST_SRC:.cpp=.o)
# -----------------------

.PHONY: all test clean


# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile app
main.o: main.cpp Math/Matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# Build AND run the tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Link tests
$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(TEST_OBJ) -o $@ $(LDFLAGS)


# Compile tests
Tests/test_matrix.o: Test/test_matrix.cpp Math/Matrix.hpp external/doctest/doctest.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# Remove build artifacts
clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)




