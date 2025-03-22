CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = ascii.out

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
