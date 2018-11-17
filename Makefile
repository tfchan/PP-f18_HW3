TARGET = prime integrate
CC = mpicc
CFLAGS = -lm

.PHONY = all clean

all:build
build:${TARGET}
${TARGET}:%:%.c
	${CC} ${CFLAGS} $? -o $@
clean:
	@${RM} -f ${TARGET}
