CXX = gcc
CXXFLAGS = -w
#HEADERS = cdjpeg.h jchuff.h jconfig.h jdct.h jdhuff.h jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h jpeglib.h jversion.h transupp.h
#OBJS = compiled_files/jcapimin.o compiled_files/jcapistd.o compiled_files/jccoefct.o compiled_files/jccolor.o compiled_files/jcdctmgr.o compiled_files/jchuff.o compiled_files/jcinit.o compiled_files/jcmainct.o compiled_files/jcmarker.o compiled_files/jcmaster.o compiled_files/jcomapi.o compiled_files/jcparam.o compiled_files/jcphuff.o compiled_files/jcprepct.o compiled_files/jcsample.o compiled_files/jctrans.o compiled_files/jdapimin.o compiled_files/jdapistd.o compiled_files/jdatadst.o compiled_files/jdatasrc.o compiled_files/jdcoefct.o compiled_files/jdcolor.o compiled_files/jddctmgr.o compiled_files/jdhuff.o compiled_files/jdinput.o compiled_files/jdmainct.o compiled_files/jdmarker.o compiled_files/jdmaster.o compiled_files/jdmerge.o compiled_files/jdphuff.o compiled_files/jdpostct.o compiled_files/jdsample.o compiled_files/jdtrans.o compiled_files/jerror.o compiled_files/jfdctflt.o compiled_files/jfdctfst.o compiled_files/jfdctint.o compiled_files/jidctflt.o compiled_files/jidctfst.o compiled_files/jidctint.o compiled_files/jidctred.o compiled_files/jmemansi.o compiled_files/jmemmgr.o compiled_files/jquant1.o compiled_files/jquant2.o compiled_files/jutils.o compiled_files/rdbmp.o compiled_files/rdcolmap.o compiled_files/rdgif.o compiled_files/rdppm.o compiled_files/rdrle.o compiled_files/rdswitch.o compiled_files/rdtarga.o compiled_files/wrbmp.o compiled_files/wrgif.o compiled_files/wrppm.o compiled_files/wrrle.o compiled_files/wrtarga.o


BUILDDIR = ./compiled_files
LIB_PATH = ../library/compiled_files/libjpeg.so
FILE1 = cdjpeg.o
EXECUTABLE2 = cjpeg
EXECUTABLE3 = djpeg
HEADERS = $(wildcard ../library/*.h)
OBJECTS = $(patsubst %.c,  $(BUILDDIR)/%.o, $(wildcard *.c))

all: dir $(BUILDDIR)/$(EXECUTABLE2) $(BUILDDIR)/$(EXECUTABLE3)

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXECUTABLE2): $(BUILDDIR)/$(EXECUTABLE2).o $(BUILDDIR)/$(FILE1)
	$(CXX) $< $(LIB_PATH) $(BUILDDIR)/$(FILE1) -o $@

$(BUILDDIR)/$(EXECUTABLE3): $(BUILDDIR)/$(EXECUTABLE3).o $(BUILDDIR)/$(FILE1)
	$(CXX) $< $(LIB_PATH) $(BUILDDIR)/$(FILE1) -o $@

$(OBJECTS): $(BUILDDIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -I../library/ -o $@

release: CXXFLAGS += -O3
release: all

clean:
	rm -rf $(BUILDDIR)
