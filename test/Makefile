include ../fly/define.mk

default:all

SUBDIRS=`ls -d */`

TARGET=main

all:main.cpp
	clang++ main.cpp -I../fly -lfly_base -L../fly/base -o ${TARGET}

lib:
	cd ../fly && make all 

cleanlib:
	cd ../fly && make clean

clean:
	rm ${TARGET}
	
