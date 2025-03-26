# Compiler settings
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Targets
TARGET = ascii85
TEST_TARGET = ascii85_test

# Source files
SRCS = ascii85.cpp main.cpp
TEST_SRCS = ascii85.cpp gtests.cpp

# Default target (build main program)
all: $(TARGET)

# Build main program
$(TARGET): $(SRCS) ascii85.h
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Build and run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Build test program
$(TEST_TARGET): $(TEST_SRCS) ascii85.h
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) $(GTEST_FLAGS) -o $(TEST_TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all test clean