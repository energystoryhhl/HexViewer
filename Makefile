
CC = g++

SRC_DIR = ./
	
SRCS := $(wildcard $(SRC_DIR )*.cpp *.c)

$(warning "src: $(SRCS)")

OBJS := $(subst .cpp,.o, $(SRCS))
$(warning "obj: $(OBJS)")
	
INC := $(SRC_DIR)

CFLAGS := $(EXTRA_CFLAGS)
CFLAGS += -I $(INC ) 

LIB_DIR := 
LIB_NAME := $(LIB_DIR)
LDFLAGS := -libpath:$(LIB_NAME)

OUT_DIR := 
TARGET= $(OUT_DIR)hexheader.exe

$(TARGET):$(SRCS)
	g++ main.cpp HexFile.cpp Encrypt.cpp crc.cpp  Sha256Calc.c yf_uid_aes.c -o hexviewer.exe

clean:
	rm -rf *.exe