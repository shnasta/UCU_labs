CXX = g++
CXXFLAGS = -w

all: dir_compiled_files demonstration

dir_compiled_files:
	mkdir -p compiled_files

demonstration: demonstration.o
	$(CXX) compiled_files/demonstration.o ../library/compiled_files/libmystring.so -o compiled_files/demonstration
demonstration.o: demonstration.cpp
	$(CXX) $(CXXFLAGS) -c demonstration.cpp -o compiled_files/demonstration.o

.PHONY: clean release

release: CXXFLAGS += -O3
release: all

clean:
	rm -rf ./compiled_files
