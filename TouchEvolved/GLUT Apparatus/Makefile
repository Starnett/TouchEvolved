###########################################################
# Project 1 Makefile

CC = gcc
CFLAGS = -Wall -ggdb
INCLUDE = -I/usr/include
LIBDIR = -L/usr/lib/x86_64-linux-gnu
# Libraries that use native graphics hardware --
LIBS = -lglut -lGLU -lGL -lpthread -lm

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework GLUT
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm

canvas: canvas.c drawing.c drawing.h vrml.c vrml.h mouse.c mouse.h
	${CC} ${CFLAGS} ${INCLUDE} -o canvas ${LIBDIR} canvas.c drawing.c vrml.c mouse.c ${LIBS}

clean:
	rm -f canvas *.o core
