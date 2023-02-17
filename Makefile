# Makefile
# This is the basic Makefile for Unix-like systems
# It works for Linux and MacOS

CFLAGS = -O
# If you have an old version of flex that defines yyleng
# as a macro rather than a variable, uncomment the following line:
# CFLAGS = -O -DNO_YYLENG_VAR

SRCS = mf2t.c midifile.c midifile.h t2mf.c t2mf.h \
       t2mf.fl t2mflex.c yyread.c getopt.h version.h\
       README.TXT Makefile makefile.st makefile.bcc makefile.msc makefile.wcc\
       tests/example1.mid tests/example1.txt tests/example2.mid tests/example2.txt \
       tests/example3.mid tests/example3.txt tests/example4.mid tests/example4.txt \
       tests/example5.mid tests/example5.txt

EXECS = mf2t t2mf
DOCS = README.TXT

all:	$(EXECS)

t2mf:	midifile.o t2mf.o t2mf.h t2mflex.o
	$(CC) t2mf.o midifile.o t2mflex.o -o t2mf

t2mf.o: t2mf.c t2mf.h getopt.h

#t2mflex.c: t2mf.fl
#	flex -is -Ce t2mf.fl
#	mv lex.yy.c t2mflex.c

t2mflex.o: t2mflex.c t2mf.h

mf2t:	midifile.o mf2t.o
	$(CC) mf2t.o midifile.o -o mf2t

mf2t.o: mf2t.c getopt.h

tar:	
	tar cf mf2t.tar $(SRCS) $(EXECS) $(DOCS)
	compress mf2t.tar

zip:	$(SRCS)  $(EXECS)
	zip -9 mf2tsrc $(SRCS) $(EXECS)

dist:	 $(EXECS) $(DOCS)
	zip -9 mf2t $(EXECS) $(DOCS)

clean:
	rm -f mf2t t2mf *.o mf2tsrc.zip mf2t.zip
