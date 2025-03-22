# Makefile для компиляции main.cpp в ascii.out

# Компилятор
CXX = g++

# Флаги компилятора
CXXFLAGS = -std=c++11 -Wall -Wextra

# Целевой исполнимый файл
TARGET = ascii.out

# Правило по умолчанию
all: $(TARGET)

# Правило для создания ascii.out из main.cpp
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

# Правило для очистки
clean:
	rm -f $(TARGET)
