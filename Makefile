SOURCES := main.c
OBJS = $(SOURCES:.c=.o)
EXES = $(SOURCES:.c=.exe)
# lib for win sound api
LIBS = -lwinmm

RESOURCE_COMPILER_UTILITY = windres
RESOURCE_COMPILER_FLAGS = -O coff
RESOURCE_SCRIPT_FILE = res/resources.rc
RESOURCE_OUTPUT_FILE = res/resources.res
RESOURCE_LINK_FILE =

build : res $(EXES)

%.exe : %.o 
	gcc $^ $(RESOURCE_OUTPUT_FILE) $(LIBS) -o $@

%.o : %.c
	gcc -g -c $^ -o $@

res : RESOURCE_LINK_FILE += RESOURCE_OUTPUT_FILE
res : $(RESOURCE_OUTPUT_FILE)

$(RESOURCE_OUTPUT_FILE) : $(RESOURCE_SCRIPT_FILE)
	$(RESOURCE_COMPILER_UTILITY) $^ $(RESOURCE_COMPILER_FLAGS) -o $@

clear : 
	rm -f $(EXES)
	rm -f $(OBJS)
	rm -f $(RESOURCE_OUTPUT_FILE)