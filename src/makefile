include ../config

ROOTDIR=..
LIBDIR=$(ROOTDIR)/lib
INCLUDE=-I$(ROOTDIR)/include -I$(PREFIX)/include

CC=gcc
CFLAGS=-Wall -fPIC -O3 $(INCLUDE) -funroll-loops
LD=gcc
LDFLAGS=-shared
SIGNUP=echo "ZX11 ver."$(VERSION)" Copyright (C) 1999 Tomomichi Sugihara (Zhidao)" >>

OBJ=zxutil.o zxfont.o zxinput.o\
	zxpixel.o zximage.o zximage_pnm.o

ifeq ($(CONFIG_USE_WIDGET),y)
	OBJ += zxwidget.o
endif

ifeq ($(CONFIG_USE_XEXT),y)
	CFLAGS += -D__ZX11_USE_XEXT
endif

ifeq ($(CONFIG_USE_XPM),y)
	OBJ += zxpm.o zxsprite.o
	CFLAGS += -D__ZX11_USE_XPM
endif

ifeq ($(CONFIG_USE_PNG),y)
	OBJ += zximage_png.o
	CFLAGS += -D__ZX11_USE_PNG
endif

ifeq ($(CONFIG_USE_TIFF),y)
	OBJ += zximage_tiff.o
	CFLAGS += -D__ZX11_USE_TIFF
endif

ifeq ($(CONFIG_USE_JPEG),y)
	OBJ += zximage_jpeg.o
	CFLAGS += -D__ZX11_USE_JPEG
endif

ifeq ($(CONFIG_USE_DIB),y)
	OBJ += zximage_dib.o
	CFLAGS += -D__ZX11_USE_BMP
endif

ifeq ($(CONFIG_USE_MAG),y)
	OBJ += zximage_mag.o
	CFLAGS += -D__ZX11_USE_MAG
endif

ifeq ($(CONFIG_USE_PDT),y)
	OBJ += zximage_pdt.o
	CFLAGS += -D__ZX11_USE_PDT
endif

DLIB=libzx11.so

TARGET=$(DLIB)

all: $(TARGET)

$(DLIB): $(OBJ)
	@echo " LD	" $^
	@$(LD) $(LDFLAGS) -o $@ $^ > /dev/null; $(SIGNUP) $@
	-@mv $@ $(LIBDIR)
%.o: %.c
	@echo " CC	" $<
	@$(CC) $(CFLAGS) -c $< > /dev/null
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ \#* $(TARGET)
