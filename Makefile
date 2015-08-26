CC = clang
CFLAGS := $(CFLAGS) -W

.PHONY: clean distclean

OBJS = main.o env.o infer.o node.o type.o
EXEC = main.out

DEBUG ?= 0
CFLAGS += -DDEBUG=$(DEBUG)

# not main to not modify .gitignore (and have it a bit more generic)
${EXEC}: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c env.h infer.h node.h type.h
env.o: env.c env.h infer.h mem.h type.h
infer.o: infer.c infer.h env.h node.h type.h
node.o: node.c node.h mem.h
type.o: type.c type.h mem.h

clean:
	rm -rf *.o

distclean: clean
	rm -f *.out

