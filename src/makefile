include Makefile.inc

DIR1	= rpgplayer
DIRC    = common
EXE1	= rpg
EXE2	= creator
OBJS1	= rpg.o
OBJSC   = files.o

all : $(EXE1)

$(EXE1) : $(OBJS1) $(OBJSC)
	$(ECHO) $(LD) $(OBJS1) $(OBJSC) -o $(EXE1)
	$(LD) $(DIR1)/$(OBJS1) $(DIRC)/$(OBJSC) -o $(EXE1)

$(OBJS1) : force_look
	$(ECHO) looking into subdir $(DIR1): $(MAKE) $(MFLAGS)
	cd $(DIR1); $(MAKE) $(MFLAGS)
	$(ECHO) Returning to top level

$(OBJSC) : force_look
	$(ECHO) looking into subdir $(DIRC): $(MAKE) $(MFLAGS)
	cd $(DIRC); $(MAKE) $(MFLAGS)
	$(ECHO) Returning to top level

clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(EXE1) $(EXE2) $(OBJS1)
	cd $(DIR1); $(MAKE) clean
	cd $(DIRC); $(MAKE) clean

force_look :
	true
