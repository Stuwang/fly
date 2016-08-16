ROOTDIR=$(shell pwd)

include define.mk

.DEFAULT: all

SUBDIRS=`ls -d */`

all:
	$(call make_subdir , all) 

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf
