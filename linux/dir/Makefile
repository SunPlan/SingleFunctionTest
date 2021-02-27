.PHONY : all clean
 
DEBUG := YES
 
CC := gcc
CXX := g++
LD := g++
AR := ar rc
 
HOME_PATH := $(realpath .)
SOURCE_PATH := ${HOME_PATH}/src
INC_PATH := -I${HOME_PATH}/src
DEP_PATH := ${HOME_PATH}/dep
SRCS := $(wildcard ${SOURCE_PATH}/*.c) $(wildcard ${SOURCE_PATH}/*.cpp)
LIB_PATH := -L${HOME_PATH}/lib
 
DEBUG_CFLAGS := -g -O0 -Wall -Wconversion -Wuninitialized -D_DEBUG -m64 -pg
RELEASE_CFLAGS := -O3 -m64
DEBUG_CXXFLAGS := ${DEBUG_CFLAGS}
RELEASE_CXXFLAGS := ${RELEASE_CFLAGS}
DEBUG_LDFLAGS := --static
RELEASE_LDFLAGS := --static
 
ifeq (YES, ${DEBUG})
    CFLAGS := ${DEBUG_CFLAGS}
    CXXFLAGS := ${DEBUG_CXXFLAGS}
    LDFLAGS := ${DEBUG_LDFLAGS}
    BIN_PATH := ${HOME_PATH}/bin/debug
    OBJ_PATH := ${HOME_PATH}/obj/debug
else
    CFLAGS := ${RELEASE_CFLAGS}
    CXXFLAGS := ${RELEASE_CXXFLAGS}
    LDFLAGS := ${RELEASE_LDFLAGS}
    BIN_PATH := ${HOME_PATH}/bin/release
    OBJ_PATH := ${HOME_PATH}/obj/release
endif
 
BIN := ${BIN_PATH}/IEM
LIBS := -Wl,-dn  -Wl,-dn  -Wl,-dn -lpthread
 
OBJS := $(patsubst %,${OBJ_PATH}/%.o,$(basename ${notdir ${SRCS}}))
DEPS := $(patsubst %,${DEP_PATH}/%.d,$(basename ${notdir ${SRCS}}))
 
all : ${BIN_PATH} ${OBJ_PATH} ${DEP_PATH} ${BIN}
 
${BIN}:${OBJS}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${LIB_PATH} ${LIBS}
 
${BIN_PATH}:
	mkdir -p ${BIN_PATH}
${OBJ_PATH}:
	mkdir -p ${OBJ_PATH}
${DEP_PATH}:
	mkdir ${DEP_PATH}
 
${DEP_PATH}/%.d : ${SOURCE_PATH}/%.c
	@set -e;\
	rm -f $@;\
	${CC} -MM ${CFLAGS} ${INC_PATH} $< > $@.tmp;\
	sed 's,\($*\)\.o[ :]*,${OBJ_PATH}/\1.o $@ : ,g' < $@.tmp > $@;\
	rm -f $@.tmp
 
${DEP_PATH}/%.d : ${SOURCE_PATH}/%.cpp
	@set -e;\
	rm -f $@;\
	${CC} -MM ${CFLAGS} ${INC_PATH} $< > $@.tmp;\
	sed 's,\($*\)\.o[ :]*,${OBJ_PATH}/\1.o $@ : ,g' < $@.tmp > $@;\
	rm -f $@.tmp
 
-include ${DEPS}
 
${OBJ_PATH}/%.o : ${SOURCE_PATH}/%.c
	${CC} -c ${CFLAGS} ${INC_PATH} $< -o ${OBJ_PATH}/${basename ${notdir $@}}.o
 
# ${OBJ_PATH}/%.o : ${SOURCE_PATH}/%.cpp
# 	${CXX} -c ${CXXFLAGS} ${INC_PATH} $< -o ${OBJ_PATH}/${basename ${notdir $@}}.o
 
clean:
	-rm -f ${HOME_PATH}/dep/*.d
	-rm -f ${HOME_PATH}/obj/debug/*.o
	-rm -f ${HOME_PATH}/obj/release/*.o
	-rm -f ${HOME_PATH}/bin/debug/*
	-rm -f ${HOME_PATH}/bin/release/*