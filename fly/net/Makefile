# ROOTDIR=/home/w/git/mynet/fly/fly
ROOTDIR=$(shell pwd)/..

include ../define.mk

OBJ=SocketOps.o Poller.o Channel.o EventLoop.o Accepter.o TcpConnection.o help.o Timer.o TimerQueue.o

STATIC_LIB=libfly_net.a

.DEFAULT:all

SUBDIRS=`ls -d */`

all:${OBJ}
#	${AR} ${AR_FLAG} ${STATIC_LIB}  ${OBJ}

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf \
	rm ${STATIC_LIB} -rf
