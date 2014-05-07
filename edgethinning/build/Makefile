CC = g++

BIN_NAME = thin

OUTPUT_DIR = ../bin

SRC_DIR = ../src

ifeq ($(OS), Windows_NT)
  GLEXT = gtkglext-win32-1.0
else
  GLEXT = gtkglext-x11-1.0
endif

ifeq ($(DBG), Yes)
  CFLAGS = -g -Wall -fopenmp
  BUILD_DIR = debug
else
  CFLAGS = -Wall -fopenmp
  BUILD_DIR = release
endif

all: build

build:
	$(CC) $(SRC_DIR)/*.cpp $(CFLAGS) -o $(OUTPUT_DIR)/$(BUILD_DIR)/$(BIN_NAME)

clean:
	rm -f $(OUTPUT_DIR)/$(BUILD_DIR)/$(BIN_NAME)

rebuild: clean build
