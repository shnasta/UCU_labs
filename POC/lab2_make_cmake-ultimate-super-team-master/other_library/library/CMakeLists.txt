cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME cmake_new_lib)
project($(PROJECT_NAME) C)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

set(NEW_LIB jccoefct.c  jcmainct.c  jcphuff.c   jdatadst.c  jdhuff.c    jdmerge.c   jerror.h    jidctint.c  jmorecfg.h  jversion.h  rdswitch.c  wrrle.c
Makefile        jccolor.c   jcmarker.c  jcprepct.c  jdatasrc.c  jdhuff.h    jdphuff.c   jfdctflt.c  jidctred.c  jpegint.h   rdbmp.c     rdtarga.c   wrtarga.c
cderror.h       jcdctmgr.c  jcmaster.c  jcsample.c  jdcoefct.c  jdinput.c   jdpostct.c  jfdctfst.c  jinclude.h  jpeglib.h   rdcolmap.c  transupp.h
cdjpeg.h        jchuff.c    jcomapi.c   jctrans.c   jdcolor.c   jdmainct.c  jdsample.c  jfdctint.c  jmemansi.c  jquant1.c   rdgif.c     wrbmp.c
jcapimin.c      jchuff.h    jconfig.h   jdapimin.c  jdct.h      jdmarker.c  jdtrans.c   jidctflt.c  jmemmgr.c   jquant2.c   rdppm.c     wrgif.c
jcapistd.c      jcinit.c    jcparam.c   jdapistd.c  jddctmgr.c  jdmaster.c  jerror.c    jidctfst.c  jmemsys.h   jutils.c    rdrle.c     wrppm.c)
add_library(cmake_new_lib.lib SHARED ${NEW_LIB})
