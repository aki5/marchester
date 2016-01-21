
CFLAGS=-O3 -ffast-math -W -Wall -fomit-frame-pointer -Wno-unused-function
#CFLAGS=-g

marchester: main.o stltext.o stlbin.o fieldfunc.o marchcube.o svdcmp.o
	$(CC) -o $@  main.o stltext.o stlbin.o fieldfunc.o marchcube.o svdcmp.o

clean:
	rm -f marchester *.o

