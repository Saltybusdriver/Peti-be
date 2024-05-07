# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source files
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = my_game

# Libraries
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf


CXXFLAGS += -I/usr/include/SDL2


# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
