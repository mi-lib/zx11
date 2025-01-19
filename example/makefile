CFLAGS=`zx11-config --cflags`
LINK=`zx11-config -l`

TARGET=$(shell ls *.c | xargs -i basename {} .c | tr -s "\n" " ")

CC=gcc
CFLAGS+=-Wall -O3 -funroll-loops -g

all: $(TARGET)
%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
clean:
	rm -f *.o *~ core *test
