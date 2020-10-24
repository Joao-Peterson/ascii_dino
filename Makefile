SOURCES := main.c
OBJS = $(SOURCES:.c=.o)
EXES = $(SOURCES:.c=.exe)

build : $(EXES)

%.exe : %.o
	gcc $^ -o $@

%.o : %.c
	gcc -g -c $^ -o $@

clear : 
	rm -f $(EXES)
	rm -f $(OBJS)