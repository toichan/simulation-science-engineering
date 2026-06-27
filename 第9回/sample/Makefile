# 拡散・移流方程式用 Makefile
PACKAGE  = diffadv
#SRCS     = diffusion.c diagnostics.c diagnostics.h upwind.c ftcs.c
#OBJS     = diffusion.o diagnostics.o upwind.o

#FILES	 = Makefile README plot.gp $(SRCS)
FILES	 = Makefile README plot.gp *.c *.h
VER	 = `date +%y%m%d`

CC	 = gcc

# make debug=on で次が true
ifeq ($(debug),on)
  CFLAGS += -g -Wall
else
  CFLAGS += -O3
endif

CINCS	 = 
CLIBS	 = -lm


# suffix rules
.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $(CINCS) $<


# main programs
diffusion: diffusion.o diagnostics.o
	$(CC) -o $@ $^ $(CLIBS)

upwind: upwind.o diagnostics.o
	$(CC) -o $@ $^ $(CLIBS)

ftcs: ftcs.o diagnostics.o
	$(CC) -o $@ $^ $(CLIBS)

# object files
diffusion.o: diagnostics.o diagnostics.h
upwind.o: diagnostics.o diagnostics.h
ftcs.o: diagnostics.o diagnostics.h


# command
# make clean で以下のルールに従ってファイルが削除される
# make tar で $(FILES) のファイルが tar-bzip される
# make test_make で変数の表示
clean:
	rm -f *~ *.o *.DAT *.dat TEMP *.mod

tar:
	@echo $(PACKAGE)-$(VER) > .package
	@-rm -fr `cat .package`
	@mkdir `cat .package`
	@ln $(FILES) `cat .package`
	tar cvf - `cat .package` | bzip2 -9 > `cat .package`.tar.bz2
	@-rm -fr `cat .package` .package

test_make:
	@echo PACKAGE=$(PACKAGE), debug=$(debug), CC=$(CC)
	@echo CFLAGS=$(CFLAGS), CINCS=$(CINCS), CLIBS=$(CLIBS)
