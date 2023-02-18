# Makefile
# This is the basic Makefile for Unix-like systems
# It works for Linux and MacOS

CFLAGS = -O
LDFLAGS =

SRCS = mf2t.c midifile.c midifile.h t2mf.c t2mf.h \
       t2mf.fl t2mflex.c yyread.c getopt.h version.h\
       README.TXT Makefile makefile.st makefile.bcc makefile.msc makefile.wcc\
       tests/example1.mid tests/example1.txt tests/example2.mid tests/example2.txt \
       tests/example3.mid tests/example3.txt tests/example4.mid tests/example4.txt \
       tests/example5.mid tests/example5.txt

DOCS = README

LIBS = midifile.o

MF2TPROG = mf2t
MF2TOBJS = mf2t.o

T2MFPROG = t2mf
T2MFOBJS = t2mf.o t2mflex.o

EXECS = $(MF2TPROG) $(T2MFPROG)
OBJS = $(MF2TOBJS) $(T2MFOBJS)

all: $(EXECS)

$(MF2TPROG): $(MF2TOBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $(MF2TPROG) $(MF2TOBJS) $(LIBS)

mf2t.o: midifile.h version.h getopt.h mf2t.c

$(T2MFPROG): $(T2MFOBJS) $(LIBS)
	$(CC) $(LDFLAGS) -o $(T2MFPROG) $(T2MFOBJS) $(LIBS)

t2mf.o: midifile.h version.h getopt.h t2mf.c

midifile.o: midifile.h midifile.c

#t2mflex.c: t2mf.fl
#	flex -is -Ce t2mf.fl
#	mv lex.yy.c t2mflex.c

t2mflex.o: t2mflex.c t2mf.h

tar:	
	tar cf mf2t.tar $(SRCS) $(EXECS) $(DOCS)
	compress mf2t.tar

zip:	$(SRCS)  $(EXECS) $(DOCS)
	zip -9 mf2tsrc $(SRCS) $(EXECS) $(DOCS)

dist:	 $(EXECS) $(DOCS)
	zip -9 mf2t $(EXECS) $(DOCS)

clean:
	rm -f $(EXECS) $(OBJS) $(LIBS) mf2tsrc.zip mf2t.zip
