ROOTDIR=$(shell pwd)/..

include ../define.mk

OBJ=Condition.o Mutex.o LogStream.o Thread.o Logger.o Buffer.o CurrentThread.o Date.o Time.o AsyncLogger.o

STATIC_LIB=libfly_base.a

.DEFAULT:all

SUBDIRS=`ls -d */`

all:${OBJ}
#	${AR} ${AR_FLAG} ${STATIC_LIB}  ${OBJ}

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf \
	rm ${STATIC_LIB} -rf
