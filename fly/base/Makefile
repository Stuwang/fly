include ../define.mk

OBJ=Condition.o Mutex.o LogStream.o Thread.o Logger.o Buffer.o

STATIC_LIB=libfly_base.a

default:all

SUBDIRS=`ls -d */`

all:${OBJ}
	${AR} ${AR_FLAG} ${STATIC_LIB}  ${OBJ}

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf \
	rm ${STATIC_LIB} -rf
