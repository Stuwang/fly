include ../fly/define.mk

default:all

SUBDIRS=`ls -d */`

TARGET=main

all:main.cpp
	clang++ main.cpp -std=c++11 -lpthread -I../fly -lfly_base -lfly_net -L../fly/base -L../fly/net -o ${TARGET}

lib:
	cd ../fly && make all 

cleanlib:
	cd ../fly && make clean

clean:
	rm ${TARGET}
	
