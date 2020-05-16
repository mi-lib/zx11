include ../../config

INCLUDE=`zx11-config -I`
LIB=`zx11-config -L`
LINK=`zx11-config -l`
DEF=`zx11-config -D`

CC=gcc
CFLAGS=-Wall -O3 $(LIB) $(INCLUDE) $(DEF)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean:
	rm -f *.o *~ core *test
