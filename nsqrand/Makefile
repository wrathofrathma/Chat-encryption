# nsqrand - random number generator and library

include config.mk

LIBBASE = libnsqrand.so
SONAME = ${LIBBASE}.${LVERSION}
LIBNAME = ${LIBBASE}.${VERSION}
SRC = nsqrand.c libnsqrand.c

all: nsqrand libnsqrand

nsqrand:
	@${CC} ${CFLAGS} -o nsqrand ${SRC} 

libnsqrand:
	@${CC} ${CFLAGS} -fPIC -c libnsqrand.c
	@${CC} ${CFLAGS} -shared -Wl,-soname,${SONAME} -o ${LIBNAME} libnsqrand.o

clean:
	@echo cleaning house!
	@rm -f ${LIBNAME} nsqrand *.o

install: all
	@echo installing executable to ${DESTDIR}${PREFIX}/bin
	@cp nsqrand ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/nsqrand
	@echo installing library to ${DESTDIR}${PREFIX}/lib
	@cp ${LIBNAME} ${DESTDIR}${PREFIX}/lib
	@chmod 755 ${DESTDIR}${PREFIX}/lib/${LIBNAME}
	@echo installing header to ${DESTDIR}${PREFIX}/include
	@cp libnsqrand.h ${DESTDIR}${PREFIX}/include
	@chmod 644 ${DESTDIR}${PREFIX}/include/libnsqrand.h
	@echo creating links and running ldconfig
	@ldconfig -n ${DESTDIR}${PREFIX}/lib
	@ln -s ${DESTDIR}${PREFIX}/lib/${LIBNAME} ${DESTDIR}${PREFIX}/lib/${LIBBASE}
	@echo installation complete!

uninstall:
	@echo removing executable from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/nsqrand 
	@echo removing libs from ${DESTDIR}${PREFIX}/lib
	@rm -f ${DESTDIR}${PREFIX}/lib/${LIBNAME}
	@echo removing header file from ${DESTDIR}${PREFIX}/include
	@rm -f ${DESTDIR}${PREFIX}/include/libnsqrand.h
	@echo cleaning up links
	@rm -f ${DESTDIR}${PREFIX}/lib/${SONAME}
	@rm -f ${DESTDIR}${PREFIX}/lib/${LIBBASE}
	@echo uninstallation complete!

.PHONY: all nsqrand libnsqrand clean install uninstall
