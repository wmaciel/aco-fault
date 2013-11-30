CC = g++

BIN_NAME = sinteticDataCreator

OUTPUT_DIR = ../bin

SRC_DIR = ../src

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
