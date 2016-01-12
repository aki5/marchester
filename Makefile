
CFLAGS=-O3 -ffast-math -fslp-vectorize-aggressive -W -Wall -fomit-frame-pointer -Wno-unused-function

marchester: main.o stltext.o stlbin.o fieldfunc.o marchcube.o
	$(CC) -o $@  main.o stltext.o stlbin.o fieldfunc.o marchcube.o

clean:
	rm -f marchester *.o

