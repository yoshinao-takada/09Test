CC = cc
DEBUG_CC_FLAGS = -c -g -I../Lib
RELEASE_CC_FLAGS = -c -O2 -I.
DEBUG_BIN_DIR = ../Debug/Bin
RELEASE_BIN_DIR = ../Release/Bin
DEBUG_OBJ_DIR = ../Debug/O
RELEASE_OBJ_DIR = ../Release/O
DEBUG_TARGET = $(DEBUG_BIN_DIR)/BMTest.exe
RELEASE_TARGET = $(RELEASE_BIN_DIR)/BMTest.exe
DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/BMTest_Main.o \
	$(DEBUG_OBJ_DIR)/BMDLNodeUT.o $(DEBUG_OBJ_DIR)/BMRingBufUT.o \
	$(DEBUG_OBJ_DIR)/BMTickUT.o $(DEBUG_OBJ_DIR)/BMEvUT.o \
	$(DEBUG_OBJ_DIR)/BMSystick.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/BMTest_Main.o \
	$(RELEASE_OBJ_DIR)/BMDLNodeUT.o $(RELEASE_OBJ_DIR)/BMRingBufUT.o \
	$(RELEASE_OBJ_DIR)/BMTickUT.o $(RELEASE_OBJ_DIR)/BMEvUT.o \
	$(RELEASE_OBJ_DIR)/BMSystick.o

.PHONY	:	debug
.PHONY	:	clean_debug

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS) $(DEBUG_BIN_DIR)/libBMLIB.a
	$(CC) -o $@ $^ -L$(DEBUG_BIN_DIR) -lBMLIB -lpthread

$(DEBUG_OBJ_DIR)/BMTest_Main.o	:	BMTest_Main.c
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMDLNodeUT.o	:	BMDLNodeUT.c ../Lib/BMDLNode.h
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMRingBufUT.o	:	BMRingBufUT.c ../Lib/BMRingBuf.h
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMTickUT.o	:	BMTickUT.c ../Lib/BMTick.h
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMEvUT.o	:	BMEvUT.c ../Lib/BMEv.h
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMSystick.o	:	BMSystick.c ../Lib/BMEv.h ../Lib/BMTick.h
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET) $(DEBUG_OBJECTS)