
CFLAGS=-O3 -ffast-math -W -Wall -fomit-frame-pointer -Wno-unused-function -D_GNU_SOURCE
#CFLAGS=-g

HFILES=\
	marchcube.h\
	mathp.h\
	vec3fp.h\
	stlbin.h\
	stltext.h\
	vec3f.h\

OFILES=\
	field.o\
	main.o\
	marchcube.o\
	stlbin.o\
	stltext.o\

LIBS=\
	-lpthread\
	-lm\

all: marchester

marchester: $(OFILES)
	$(CC) -o $@  $(OFILES) $(LIBS)

clean:
	rm -f marchester *.o

$(OFILES): $(HFILES)
