# Detect OS/platform:
include ../makefile_os.inc

CXX = gcc
FASM = ./fasmg.x64

TARGETS = call_from_c call_from_a
BIN_DIR = bin
SRC_DIR = src

# If OS is not Linux raise error:
ifneq ($(USED_OS), LINUX)
$(error "This makefile is for Linux only. Current OS is $(USED_OS)")
endif

all: ${BIN_DIR} ${TARGETS}
	echo "Build targets: ${TARGETS}"

${BIN_DIR}:
	mkdir -p ${BIN_DIR}

call_from_c: ${SRC_DIR}/main_c.o ${SRC_DIR}/func.o
	${CXX} $^ -o ${BIN_DIR}/$@

call_from_a: ${SRC_DIR}/main_a.o ${SRC_DIR}/func.o
	${CXX} $^ -o ${BIN_DIR}/$@ -no-pie

.INTERMEDIATE: ${SRC_DIR}/main_c.o ${SRC_DIR}/main_a.o ${SRC_DIR}/func.o

${SRC_DIR}/%.o: ${SRC_DIR}/%.c
	${CXX} $^ -o $@ -c

${SRC_DIR}/%.o: ${SRC_DIR}/%.asm
	INCLUDE=${CURDIR}/../x86/include ../${FASM} $<

.PHONY: clean
clean:
	rm -rf *.o ${BIN_DIR} || true
