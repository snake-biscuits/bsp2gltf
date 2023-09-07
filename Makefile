CXX       := g++
CFLAGS    := -ggdb
SRC_DIRS  := src src/branch src/bsp src/convert
ALL_DIRS  := $(addprefix build/,$(SRC_DIRS))
CPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
HPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.hpp))
O_FILES   := $(foreach file,$(CPP_FILES),build/$(file:.cpp=.o))

ifeq ($(OS),Windows_NT)  # NOTE: if compiling for Windows, use MSYS2 / MINGW64
    CXX      := x86_64-w64-mingw32-g++
endif

DUMMY != mkdir -p build $(ALL_DIRS)

.PHONY: all


all: build/bsp2gltf.exe

# TODO: clean

build/%.o: %.cpp $(CPP_FILES)
	$(CXX) $(CFLAGS) -c -o $@ $<

build/bsp2gltf.exe: $(O_FILES)
	$(CXX) $(CFLAGS) -o $@ $^