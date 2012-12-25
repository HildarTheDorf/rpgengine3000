include Makefile.inc

DIR1	= rpgplayer
DIR2    = rpgcreator
DIRC    = common
EXE1	= rpg
EXE2	= creator
OBJS1	= $(DIR1)/rpg.o
OBJS2   = $(DIR2)/rpgcreator.o $(DIR2)/main.o $(DIR2)/moc_rpgcreator.o
OBJSC   = $(DIRC)/files.o

all : $(EXE1) $(EXE2)

$(EXE1) : $(OBJS1) $(OBJSC)
	$(ECHO) $(LD) $(OBJS1) $(OBJSC) -o $(EXE1) $(LDFLAGS1)
	$(LD) $(OBJS1) $(OBJSC) -o $(EXE1) $(LDFLAGS1)

$(EXE2) : $(OBJS2) $(OBJSC)
	$(ECHO) $(LD) $(OBJS2) $(OBJSC) -o $(EXE2) $(LDFLAGS2)
	$(LD) $(OBJS2) $(OBJSC) -o $(EXE2) $(LDFLAGS2)

$(OBJS1) : force_look
	$(ECHO) Looking into subdir $(DIR1): $(MAKE) $(MFLAGS)
	cd $(DIR1); $(MAKE) $(MFLAGS)
	$(ECHO) Returning to top level

$(OBJS2) : force_look
	$(ECHO) Looking into subdir $(DIR2): $(MAKE) $(MFLAGS)
	cd $(DIR2); $(MAKE) $(MFLAGS)
	$(ECHO) Returning to top level

$(OBJSC) : force_look
	$(ECHO) Looking into subdir $(DIRC): $(MAKE) $(MFLAGS)
	cd $(DIRC); $(MAKE) $(MFLAGS)
	$(ECHO) Returning to top level

clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(EXE1) $(EXE2)
	cd $(DIR1); $(MAKE) clean
	cd $(DIR2); $(MAKE) clean
	cd $(DIRC); $(MAKE) clean

force_look :
	true
