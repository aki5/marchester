
CFLAGS=-O3 -mavx -ffast-math -W -Wall -fomit-frame-pointer -Wno-unused-function
#CFLAGS=-g

HFILES=\
	marchcube.h\
	vec3f8p.h\
	stlbin.h\
	stltext.h\
	vec3f.h\

OFILES=\
	fieldfunc.o\
	main.o\
	marchcube.o\
	stlbin.o\
	stltext.o\

all: marchester

marchester: $(OFILES)
	$(CC) -o $@  $(OFILES)

clean:
	rm -f marchester *.o

$(OFILES): $(HFILES)
