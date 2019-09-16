
CC = g++

MBEDTLS_SRC_DIR = ./mbedtls/library/
MBEDTLS_SRC = $(wildcard $(MBEDTLS_SRC_DIR)*.c)
$(warning "MBEDTLS_SRC: $(MBEDTLS_SRC)")

SRC_DIR = ./
SRCS = $(wildcard $(SRC_DIR )*.cpp *.c)
SRCS += $(MBEDTLS_SRC)

$(warning "SRCS: $(SRCS)")

OBJS := $(subst .cpp,.o, $(SRCS))
$(warning "obj: $(OBJS)")

MBEDTLS_INC = ./mbedtls/include/
INC += $(MBEDTLS_INC)

EXTRA_CFLAGS = -static

CFLAGS := $(EXTRA_CFLAGS)
CFLAGS += -I $(INC) 

LIB_DIR := 
LIB_NAME := $(LIB_DIR)
LDFLAGS := -fpermissive

OUT_DIR := 
TARGET= $(OUT_DIR)hexheader.exe

$(TARGET):$(SRCS)
	$(CC) -o $@ $^ $(CFLAGS)  $(LDFLAGS)
	#g++ main.cpp HexFile.cpp Encrypt.cpp crc.cpp  Sha256Calc.c yf_uid_aes.c -o hexviewer.exe

clean:
	rm -rf *.exe