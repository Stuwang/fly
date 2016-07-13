include ../define.mk

OBJ=SocketOps.o

STATIC_LIB=libfly_net.a

default:all

SUBDIRS=`ls -d */`

all:${OBJ}
	${AR} ${AR_FLAG} ${STATIC_LIB}  ${OBJ}

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf
