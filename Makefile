.PHONY: clean install

# You may remove `NEED_STRL' from CFLAGS
# and `strl.o' from OBJ if your system
# provides strlcpy and strlcat.

CC = cc
PREFIX = /usr/local/bin
CFLAGS = -std=c99 -pedantic -Wall -D_DEFAULT_SOURCE -D_BSD_SOURCE -DNEED_STRL
SHELL_TARGETS = bash ksh zsh

OBJ = segment.o main.o pwd.o git.o strl.o
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
	@SH_TG="${SHELL_TARGETS}" && for s in $$SH_TG; \
		do ln -s pwrln ${PREFIX}/pwrln-$$s 2> /dev/null && \
		echo Added symlink ${PREFIX}/pwrln-$$s || \
		echo ${PREFIX}/pwrln-$$s already exists; done
