ROOTDIR=$(shell pwd)

include define.mk

.DEFAULT: all

SUBDIRS=`ls -d */`

all:
	$(call make_subdir , all) \
	${AR} ${AR_FLAG} libfly.a base/*.o net/*.o

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf
