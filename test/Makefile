include ../fly/define.mk

default:all

SUBDIRS=`ls -d */`

all:main.cpp
	clang++ main.cpp -I../fly -lfly_base -L../fly/base

clean:
	
