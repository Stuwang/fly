AR=ar

AR_FLAG=rcs

CCX=clang++

INCLUDE=${ROOTDIR}

CXX_FLAG=-O3 -Werror -Wall -g -std=c++11

INCLUDE_FLAG=-I${INCLUDE} -I.

LD_FLAG=

%.o:%.cpp
	${CCX} ${CXX_FLAG} ${INCLUDE_FLAG} ${LD_FLAG}  $^ -c -o $@

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
