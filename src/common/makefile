include ../Makefile.inc

CFLAGS	= $(PRJCFLAGS)
SOURCES	= files.c
OBJS = $(SOURCES:.c=.o)


all : $(OBJS)

.c.o:
	$(ECHO) $(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(ECHO) cleaning up in common
	-$(RM) -f $(OBJS)

force_look :
	true
