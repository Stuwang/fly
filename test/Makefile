include ../fly/define.mk

default:all

SUBDIRS=`ls -d */`

TARGET=main

all:main.cpp
	$(CCX) $(CXX_FLAG) main.cpp -lpthread -I../fly -lfly -L../fly/ -o ${TARGET}

lib:
	cd ../fly && make all  && make all

cleanlib:
	cd ../fly && make clean

clean:
	rm ${TARGET}
	
