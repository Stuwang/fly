include ../fly/define.mk

default:all

SUBDIRS=`ls -d */`

TARGET=main

all:main.cpp
	clang++ main.cpp -O3 -std=c++11 -lpthread -I../fly -lfly -L../fly/ -o ${TARGET}

lib:
	cd ../fly && make all  && make all

cleanlib:
	cd ../fly && make clean

clean:
	rm ${TARGET}
	
