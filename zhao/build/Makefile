CC = g++

BIN_NAME = zhao

OUTPUT_DIR = ../bin

SRC_DIR = ../src

ifeq ($(DBG), Yes)
  CFLAGS = -g -Wall -fopenmp `pkg-config --cflags --libs gl gtk+-2.0 gtkglext-1.0 gtkglext-win32-1.0`
  BUILD_DIR = debug
else
  CFLAGS = -Wall -fopenmp `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-win32-1.0`
  BUILD_DIR = release
endif

all: build

build:
	$(CC) $(SRC_DIR)/*.cpp $(CFLAGS) -o $(OUTPUT_DIR)/$(BUILD_DIR)/$(BIN_NAME)

clean:
	rm -f $(OUTPUT_DIR)/$(BUILD_DIR)/$(BIN_NAME)

rebuild: clean build
