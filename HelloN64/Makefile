N64KITDIR	= c:/nintendo/n64kit

TARGET		= HelloN64
CODEFILES   = $(wildcard *.c)

############################################################################
# DO NOT CHANGE ANYTHING BELOW THIS LINE UNLESS YOU KNOW WHAT YOU'RE DOING #
############################################################################

include $(ROOT)/usr/include/make/PRdefs

DEBUGSYM	= -g
OPTIMIZER	= -O0

NUSYSDIR	= $(N64KITDIR)/nusys
NUSYSINC	= $(NUSYSDIR)/include
NUSYSLIB	= $(NUSYSDIR)/lib
NUSTDINCDIR  = $(N64KITDIR)/nustd/include
NUSTDLIBDIR  = $(N64KITDIR)/nustd/lib

NUOBJ		= $(NUSYSLIB)/nusys.o
CODEOBJECTS	= $(CODEFILES:.c=.o) $(NUOBJ)

CUSTFLAGS	= 
LCINCS		= -I$(NUSYSINC) -I$(NUSTDINCDIR)
LCOPTS		= -G 0 $(DEBUGSYM) $(CUSTFLAGS)
LDFLAGS		= -L$(ROOT)/usr/lib -L$(ROOT)/usr/lib/PR -L$(NUSYSLIB) -L$(NUSTDLIBDIR) -lnusys_d -lgultra_d -lnustd_d -L$(GCCDIR)/mipse/lib -lkmc

CODESEGMENT	= codesegment.o
OBJECTS		= $(CODESEGMENT)

SYMBOL	= $(TARGET).out
ROM	    = $(TARGET).n64

default:$(ROM)

$(CODESEGMENT):	$(CODEOBJECTS)
		$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(ROM) :	$(OBJECTS)
		$(MAKEROM) spec -I$(NUSYSINC) -r $(ROM) -e $(SYMBOL)
		makemask $(ROM)

include $(ROOT)/usr/include/make/commonrules
# This space is needed or makefile errors
