include Makefile.inc

DIR1	= rpgplayer
DIRS    = $(DIR1)
EXE1	= rpg
EXE2	= creator
OBJS1	= rpg.o

all : $(EXE1)

$(EXE1) : $(OBJS1)
	$(ECHO) $(LD) $(OBJS1) -o $(EXE1)
	$(LD) $(DIR1)/$(OBJS1) -o $(EXE1)

$(OBJS1) : force_look
	$(ECHO) looking into subdir $(DIR1): $(MAKE) $(MFLAGS)
	cd $(DIR1); $(MAKE) $(MFLAGS)
	$(ECHO) Returning to top level

clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(EXE1) $(EXE2) $(OBJS1)
	cd $(DIR1); $(MAKE) clean

force_look :
	true
