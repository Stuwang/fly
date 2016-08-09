# ROOTDIR=/home/w/git/mynet/fly/fly
ROOTDIR=$(shell pwd)

include define.mk

default:all

SUBDIRS=`ls -d */`

all:
	$(call make_subdir , all) 

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf
