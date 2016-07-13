include define.mk

default:all

SUBDIRS=`ls -d */`

all:${OBJ}
	$(call make_subdir , all)

clean:
	$(call clean) \
	rm main *.o *.out ${OBJ} -rf
