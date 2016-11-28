AR=ar

AR_FLAG=rcs

# CCX=clang++
CCX=g++

INCLUDE=${ROOTDIR}

DEBUG=-g

OP_LEVEL=-O2

CXX_VERSION=-std=c++11

CXX_FLAG=$(OP_LEVEL) $(DEBUG) -Werror -Wall $(CXX_VERSION)

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
