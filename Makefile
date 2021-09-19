CFLAGS += -Iinclude
LDFLAGS += -lraylib

C_SRCS := $(wildcard */*.c)
A_SRCS := $(wildcard */*.s)

OBJS := $(C_SRCS:.c=.o)
OBJS += $(A_SRCS:.s=.o)

OUT = cgol

all: ${BUILD} ${OUT}

%.o: %.c
	clang ${CFLAGS} -c $< -o $@

${OUT}: ${OBJS}
	clang -o $@ ${OBJS} ${LDFLAGS}

$(BUILD):
	mkdir -p $@

debug:
	CFLAGS="-g -O0" ${MAKE} -B

clean:
	rm -f ${OBJS} ${OUT}

.PHONY: clean debug
