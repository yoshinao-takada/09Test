CC = cc
DEBUG_CC_FLAGS = -c -g -I.
RELEASE_CC_FLAGS = -c -O2 -I.
DEBUG_BIN_DIR = ../Debug/Bin
RELEASE_BIN_DIR = ../Release/Bin
DEBUG_OBJ_DIR = ../Debug/O
RELEASE_OBJ_DIR = ../Release/O
DEBUG_TARGET = $(DEBUG_BIN_DIR)/libBMLIB.a
RELEASE_TARGET = $(RELEASE_BIN_DIR)/libBMLIB.a
DEBUG_OBJECTS = $(DEBUG_OBJ_DIR)/BMEv.o $(DEBUG_OBJ_DIR)/BMCRC.o \
	$(DEBUG_OBJ_DIR)/BMDLNode.o $(DEBUG_OBJ_DIR)/BMRingBuf.o \
	$(DEBUG_OBJ_DIR)/BMLinBuf.o $(DEBUG_OBJ_DIR)/BMTick.o
RELEASE_OBJECTS = $(RELEASE_OBJ_DIR)/BMEv.o $(RELEASE_OBJ_DIR)/BMCRC.o \
	$(RELEASE_OBJ_DIR)/BMDLNode.o $(RELEASE_OBJ_DIR)/BMRingBuf.o \
	$(RELEASE_OBJ_DIR)/BMLinBuf.o $(RELEASE_OBJ_DIR)/BMTick.o

.PHONY	:	debug
.PHONY	:	clean_debug

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_BIN_DIR)
	$(AR) r $@ $^

$(DEBUG_OBJ_DIR)/BMEv.o	:	BMEv.c BMEv.h BMDefs.h BMDLNode.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMCRC.o	:	BMCRC.c BMCRC.h BMDefs.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMDLNode.o	:	BMDLNode.c BMDLNode.h BMDefs.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMRingBuf.o	:	BMRingBuf.c BMRingBuf.h BMDefs.h BMDLNode.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMLinBuf.o	:	BMLinBuf.c BMLinBuf.h BMDefs.h BMDLNode.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

$(DEBUG_OBJ_DIR)/BMTick.o	:	BMTick.c BMTick.h BMDefs.h BMDLNode.h
	mkdir -p $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET) $(DEBUG_OBJECTS)
