.PHONY: clean install

CC = cc
PREFIX = /usr/local/bin
CFLAGS = -std=c99 -pedantic -Wall

OBJ = segment.o main.o pwd.o
HDR = pwrln.h segments.h config.h

pwrln: ${OBJ}
	@echo LD $@
	@${CC} -o $@ ${OBJ}

.c.o: ${HDR}
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

clean:
	rm ${OBJ}

install: pwrln
	@echo Installing to ${PREFIX}/pwrln
	@cp ./pwrln ${PREFIX}/pwrln
