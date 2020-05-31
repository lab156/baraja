# Makefile for gtest examples

GOOGLE_TEST_LIB = gtest
LOC = ~/Paquetes/googletest/lib
GOOGLE_TEST_INCLUDE = ~/Paquetes/googletest/googletest/include

G++ = g++
G++_FLAGS = -c -Wall -I $(GOOGLE_TEST_INCLUDE)
LD_FLAGS = -L $(LOC) -l $(GOOGLE_TEST_LIB) -l pthread

OBJECTS = main.o tests.o 
TARGET = tests.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
