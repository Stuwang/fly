include ../define.mk

OBJ=Condition.o Mutex.o

default:all

SUBDIRS=`ls -d */`

all:${OBJ}
	ar rcs libfly_base.a ${OBJ}

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf
