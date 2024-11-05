# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude

# Libraries
LIBS = -lglfw3 -lopengl32

# Source files and executable
SRC_DIR = src
SRCS = $(SRC_DIR)/glad.c $(SRC_DIR)/helloTriangle.cpp
TARGET = helloTriangle.exe

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

