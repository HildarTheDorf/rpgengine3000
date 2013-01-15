include ../Makefile.inc

CFLAGS	= $(PRJCFLAGS)
SOURCES	= rpg.c main.c combat.c
OBJS = $(SOURCES:.c=.o)


all : $(OBJS)

.c.o:
	$(ECHO) $(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(ECHO) cleaning up in rpgplayer
	-$(RM) -f $(OBJS)

force_look :
	true
