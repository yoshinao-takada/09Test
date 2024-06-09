CC = cc
DEBUG_CC_FLAGS = -c -g -I../Lib
RELEASE_CC_FLAGS = -c -O2 -I.
DEBUG_BIN_DIR = ../Debug/Bin
RELEASE_BIN_DIR = ../Release/Bin
DEBUG_OBJ_DIR = ../Debug/O
RELEASE_OBJ_DIR = ../Release/O
DEBUG_TARGET = $(DEBUG_BIN_DIR)/BMtest.exe
RELEASE_TARGET = $(RELEASE_BIN_DIR)/BMTest.exe
DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/BMTest_Main.o \
	$(DEBUG_OBJ_DIR)/BMDLNodeUT.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/BMTest_Main.o \
	$(RELEASE_OBJ_DIR)/BMDLNodeUT.o

.PHONY	:	debug
.PHONY	:	clean_debug

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	$(CC) -o $@ $^ -L$(DEBUG_BIN_DIR) -lBMLIB -lpthread

$(DEBUG_OBJ_DIR)/BMTest_Main.o	:	BMTest_Main.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMDLNodeUT.o	:	BMDLNodeUT.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET) $(DEBUG_OBJECTS)