# Internal variables

BINARY_EXT=
SHARED_LIBRARY_EXT=
OS_NAME=
ARCH=

ifeq ($(OS_NAME),)
	ifeq ($(OS),Windows_NT)
		SHARED_LIBRARY_EXT=.dll
		BINARY_EXT=.exe
		OS_NAME=Windows
	else
		UNAME := $(shell uname -s)
		ifeq ($(UNAME),Darwin)
			SHARED_LIBRARY_EXT=.dylib
			OS_NAME=macOS
		endif
		ifeq ($(UNAME),Linux)
			SHARED_LIBRARY_EXT=.so
			OS_NAME=Linux
		endif
	endif
endif

ifeq ($(ARCH),)
	ifeq ($(OS_NAME),Windows)
		ifeq ($(PROCESSOR_ARCHITEW64),AMD64)
    		ARCH=x86_64
		else
			ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
				ARCH-x86_64
			endif
			ifeq ($(PROCESSOR_ARCHITECTURE),x86)
				ARCH=x86
			endif
		endif
	else
		PROCESSOR = $(shell uname -m)
		ifeq ($(PROCESSOR),x86_64)
			ARCH=x86_64
		endif
		ifneq ($(filter %86,PROCESSOR),)
			ARCH=x86
		endif
	endif
endif

# Config

CC=clang
AR=ar
SRC=source
OUT=build
OBJ=objects
INC=include
DEPS=dependencies
ARFLAGS=-crs
LDFLAGS=-L$(OBJ)/$(OS_NAME)/$(ARCH)
CFLAGS=-O3 -MD -pedantic -std=c17 -Werror -Wall -Wextra
TASK_COLOR=\033[1;36m
COMPILED_COLOR=\033[1;32m
RESET_COLOR=\033[m

#ifeq ($(ARCH),)
#	$(error Invalid architecture detected)
#endif

ifeq ($(OS_NAME),)
	$(warning Unknown OS)
	OS_NAME=Unknown
endif

OBJECT_FILES := $(shell find $(OBJ) -name "*.o")
BACON_ENGINE_SOURCES := $(shell find $(SRC)/BaconEngine -name "*.c")
BACON_ENGINE_OBJECTS := $(BACON_ENGINE_SOURCES:$(SRC)/BaconEngine/%.c=$(OBJ)/$(OS_NAME)/$(ARCH)/BaconEngine/%.o)
SHARED_SOURCES := $(shell find $(SRC)/SharedEngineCode -name "*.c")
SHARED_OBJECTS := $(SHARED_SOURCES:$(SRC)/SharedEngineCode/%.c=$(OBJ)/$(OS_NAME)/$(ARCH)/SharedEngineCode/%.o)
LAUNCHER_SOURCES := $(shell find $(SRC)/BaconLauncher -name "*.c")
LAUNCHER_OBJECTS := $(LAUNCHER_SOURCES:$(SRC)/BaconLauncher/%.c=$(OBJ)/$(OS_NAME)/$(ARCH)/BaconLauncher/%.o)
TEST_GAME_SOURCES := $(shell find $(SRC)/TestGame -name "*.c")
TEST_GAME_OBJECTS := $(TEST_GAME_SOURCES:$(SRC)/TestGame/%.c=$(OBJ)/$(OS_NAME)/$(ARCH)/TestGame/%.o)

# HACK: There has to be a way around that.

define make_dir
	@if [ ! -d $1 ]; then \
		echo "mkdir -p $1"; \
		mkdir -p $1; \
    fi
endef

all: launcher test_game

help:
	@echo "BaconEngine MakeFile"
	@echo ""
	@echo "Information:"
	@echo "	help: Information about each target"
	@echo ""
	@echo "Compiles:"
	@echo "	all: Compiles everything"
	@echo "	engine: Compile the engine"
	@echo "	shared: Compile the shared engine code"
	@echo "	launcher: Compiles the launcher"
	@echo "	test_game: Compiles the test game"
	@echo ""
	@echo "Misc:"
	@echo "	clean: Cleans everything up"

launcher: shared $(OUT)/$(OS_NAME)/$(ARCH)/BaconLauncher$(BINARY_EXT)

test_game: engine $(OUT)/$(OS_NAME)/$(ARCH)/TestGame/binary$(SHARED_LIBRARY_EXT)

engine: shared $(OBJ)/$(OS_NAME)/$(ARCH)/libBaconEngine.a

shared: $(OBJ)/$(OS_NAME)/$(ARCH)/libSharedEngineCode.a

clean:
	rm -rf $(OBJ)/*

# Internal targets
# BaconLauncher

$(OUT)/$(OS_NAME)/$(ARCH)/BaconLauncher$(BINARY_EXT): $(LAUNCHER_OBJECTS)
	$(call make_dir,$(shell dirname $@))
	$(CC) $(LDFLAGS) -lSharedEngineCode $^ -o $@

$(OBJ)/$(OS_NAME)/$(ARCH)/BaconLauncher/%.o: $(SRC)/BaconLauncher/%.c
	$(call make_dir,$(shell dirname $@))
	$(CC) $(CFLAGS) -I$(INC)/SharedEngineCode -c $< -o $@

# BaconEngine

$(OBJ)/$(OS_NAME)/$(ARCH)/libBaconEngine.a: $(BACON_ENGINE_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ)/$(OS_NAME)/$(ARCH)/BaconEngine/%.o: $(SRC)/BaconEngine/%.c
	$(call make_dir,$(shell dirname $@))
	$(CC) $(CFLAGS) -I$(INC)/BaconEngine -I$(SRC)/BaconEngine -I$(INC)/SharedEngineCode -I$(DEPS)/SDL/include -c $< -o $@

# TestGame

$(OUT)/$(OS_NAME)/$(ARCH)/TestGame/binary$(SHARED_LIBRARY_EXT): $(TEST_GAME_OBJECTS)
	$(call make_dir,$(shell dirname $@))
	$(CC) $(LDFLAGS) -shared -lBaconEngine -lSharedEngineCode -lSDL2 $^ -o $@

$(OBJ)/$(OS_NAME)/$(ARCH)/TestGame/%.o: $(SRC)/TestGame/%.c
	$(call make_dir,$(shell dirname $@))
	$(CC) $(CFLAGS) -I$(INC)/SharedEngineCode -I$(DEPS)/SDL/include -I$(INC)/BaconEngine -I$(SRC)/SharedEngineCode -c $< -o $@

# SharedEngineCode

$(OBJ)/$(OS_NAME)/$(ARCH)/libSharedEngineCode.a: $(SHARED_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ)/$(OS_NAME)/$(ARCH)/SharedEngineCode/%.o: $(SRC)/SharedEngineCode/%.c
	$(call make_dir,$(shell dirname $@))
	$(CC) $(CFLAGS) -I$(INC)/SharedEngineCode -I$(SRC)/SharedEngineCode -c $< -o $@

-include $(OBJECT_FILES:.o=.d)