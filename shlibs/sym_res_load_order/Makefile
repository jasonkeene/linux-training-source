#!/bin/sh

CFLAGS = -Wall -g

SHLIBS = libx2.so libx1.so liby2.so liby1.so libz3.so libz2.so libz1.so

all: ${SHLIBS} main

clean:
	rm -f ${SHLIBS} main *.o

# Unfortunately, recent Ubuntu distributions (e.g., 18-04)--and perhaps some
# other less widely used distributions--revert the ld(1) default option
# "--no-as-needed". Therefore, we must explicitly specify it here to avoid
# errors when building 'main'.

NFLAG = -Wl,--no-as-needed

libx2.so: libx2.o
	cc ${CFLAGS} ${NFLAG} -o libx2.so -shared -fPIC libx2.c
libx1.so: libx1.o libx2.so
	cc ${CFLAGS} ${NFLAG} -o libx1.so -shared -fPIC libx1.c libx2.so
liby2.so: liby2.o
	cc ${CFLAGS} ${NFLAG} -o liby2.so -shared -fPIC liby2.c
liby1.so: liby1.o liby2.so
	cc ${CFLAGS} ${NFLAG} -o liby1.so -shared -fPIC liby1.c liby2.so
libz3.so: libz3.o
	cc ${CFLAGS} ${NFLAG} -o libz3.so -shared -fPIC libz3.c
libz2.so: libz2.o libz3.so
	cc ${CFLAGS} ${NFLAG} -o libz2.so -shared -fPIC libz2.c libz3.so
libz1.so: libz1.o libz2.so
	cc ${CFLAGS} ${LDFLAGS} ${NFLAG} -o libz1.so -shared \
		-fPIC libz1.c libz2.so

main: main.o libx1.so liby1.so libz1.so
	cc ${CFLAGS} ${NFLAG} -o main main.o -Wl,-rpath-link,. \
		-L. libx1.so liby1.so libz1.so
