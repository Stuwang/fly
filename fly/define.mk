AR=ar

AR_FLAG=rcs

CCX=clang++
#CCX=g++

INCLUDE=${ROOTDIR}

CXX_FLAG=-O2 -g -Werror -Wall -std=c++11

INCLUDE_FLAG=-I${INCLUDE} -I.

# LD_FLAG=-pthread -dl

%.o:%.cpp
	${CCX} ${CXX_FLAG} ${INCLUDE_FLAG} $^ -c -o $@

# function 

define make_subdir
	@for subdir in $(SUBDIRS) ; do \
	( cd $$subdir && make -j4 ) \
	done;
endef

define clean
	@for subdir in $(SUBDIRS) ; do \
	( cd $$subdir && make clean) \
	done;
endef
