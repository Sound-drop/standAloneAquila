CXX = clang++
CXXFLAGS = -O2 -Wall --std=c++11

LIBRARY_SRC = /Users/micklin/Documents/cpp_final/aquila-src
PA_STATIC_LIB = $(LIBRARY_SRC)/libAquila.a $(LIBRARY_SRC)/lib/libOoura_fft.a
PA_INCLUDE_DIR = $(LIBRARY_SRC)

all: test

test: main.cpp
	$(CXX) $(CXXFLAGS) $< $(PA_STATIC_LIB) -I$(PA_INCLUDE_DIR) -o $@

clean:
	rm -rf test