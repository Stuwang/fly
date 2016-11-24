ROOTDIR=$(shell pwd)

include define.mk

.DEFAULT: all

SUBDIRS=`ls -d */`

all:
	$(call make_subdir , all) \
	${AR} ${AR_FLAG} libfly.a base/*.o net/*.o

pull:
	git pull origin master

clean:
	$(call clean) \
	rm main *.o *.out libfly.a ${OBJ} -rf
