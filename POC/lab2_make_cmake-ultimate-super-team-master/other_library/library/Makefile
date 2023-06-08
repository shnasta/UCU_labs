CXX = gcc
CXXFLAGS = -w

BUILDDIR = ./compiled_files
LIBRARY = libjpeg
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.c, $(BUILDDIR)/%.o, $(wildcard *.c))


all: dir $(BUILDDIR)/$(LIBRARY)

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(LIBRARY): $(OBJECTS)
	$(CXX) -shared -fPIC $^ -o $@.so
	ar rcs $@.a $^

$(OBJECTS): $(BUILDDIR)/%.o: %.c
	$(CXX) -c $(CXXFLAGS) $< -o $@ -fPIC

release: CXXFLAGS += -O3
release: all

clean:
	rm -rf $(BUILDDIR)
