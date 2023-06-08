ifeq ($(OS),Windows_NT)
	cpu = $(firstword $(subst -, ,$(shell $(CC) -dumpmachine)))
	ifeq ($(cpu),x86_64) 
		#  MinGW/MSYS2. Native would look like: ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		USED_OS = WIN64
	endif
	ifeq ($(cpu),i686)
		# Native: ($(PROCESSOR_ARCHITECTURE),x86) for native
		USED_OS = WIN32
	endif	
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		USED_OS = LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		USED_OS = OSX
	endif
##  If you need bitness	also:
#	UNAME_P := $(shell uname -p)
#	ifeq ($(UNAME_P),x86_64)
#		OSFLAG += -D AMD64
#	endif
#		ifneq ($(filter %86,$(UNAME_P)),)
#	OSFLAG += -D IA32
#		endif
#	ifneq ($(filter arm%,$(UNAME_P)),)
#		OSFLAG += -D ARM
#	endif
endif
