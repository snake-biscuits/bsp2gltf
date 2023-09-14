CXX       := g++
CXXFLAGS  := -ggdb
SRC_DIRS  := src src/branch src/bsp src/convert
ALL_DIRS  := $(addprefix build/,$(SRC_DIRS))
CPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
HPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.hpp))
O_FILES   := $(foreach file,$(CPP_FILES),build/$(file:.cpp=.o))

DUMMY != mkdir -p build $(ALL_DIRS)

.PHONY: all clean


all: build/bsp2gltf.exe

clean: rm -rf build/src/


build/%.o: %.cpp $(CPP_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/bsp2gltf.exe: $(O_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^
