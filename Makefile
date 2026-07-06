# Makefile for Indie Store (PS3 Homebrew)
# Requires PSL1GHT and ps3toolchain

APP_ID := INDI00001
TITLE := Indie Store
APP_VER := 01.00

# Arch
PPU_CC = ppu-gcc
PPU_CXX = ppu-g++
PPU_AR = ppu-ar
PPU_OBJCOPY = ppu-objcopy

# Flags
CXXFLAGS = -O2 -Wall -std=c++17 -fno-exceptions -fno-rtti
CXXFLAGS += -I$(PSL1GHT)/ppu/include -I$(PS3DEV)/portlibs/ppu/include -Isrc

LDFLAGS = -L$(PSL1GHT)/ppu/lib -L$(PS3DEV)/portlibs/ppu/lib
LIBS = -ltiny3d -lpng -ljpeg -lfreetype -lz -lcurl -lssl -lcrypto -lrsx -laudio -lnet -lnetctl -lsysutil -lsysmodule -lio -lrt -llv2 -lm

# Sources
SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/*/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Targets
TARGET = indie_store.elf

all: $(OBJ_DIR) $(TARGET) EBOOT.BIN

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/Application
	mkdir -p $(OBJ_DIR)/Graphics
	mkdir -p $(OBJ_DIR)/Network
	mkdir -p $(OBJ_DIR)/Downloader
	mkdir -p $(OBJ_DIR)/Parser
	mkdir -p $(OBJ_DIR)/Database
	mkdir -p $(OBJ_DIR)/UI
	mkdir -p $(OBJ_DIR)/Utils
	mkdir -p $(OBJ_DIR)/Input
	mkdir -p $(OBJ_DIR)/Audio
	mkdir -p $(OBJ_DIR)/Resources

$(TARGET): $(OBJS)
	$(PPU_CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(PPU_CXX) $(CXXFLAGS) -c $< -o $@

EBOOT.BIN: $(TARGET)
	make_fself $(TARGET) EBOOT.BIN

clean:
	rm -rf $(OBJ_DIR) $(TARGET) EBOOT.BIN

.PHONY: all clean
