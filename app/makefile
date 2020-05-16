include ../config

INCLUDE=-I$(PREFIX)/include -I../include -I.
LIB=-L$(PREFIX)/lib -L../lib
BINDIR=$(PREFIX)/bin

CC=gcc
CFLAGS=-ansi -Wall -O3 $(INCLUDE) $(LIB)

LINK=-lzx11
DEF=

ifeq ($(CONFIG_USE_XEXT),y)
	LINK+=-lXext
	DEF += -D__ZX11_USE_XEXT
endif

ifeq ($(CONFIG_USE_XPM),y)
	LINK+=-lXpm
	DEF += -D__ZX11_USE_XPM
endif

ifeq ($(CONFIG_USE_PNG),y)
	LINK+=-lpng -lz
	DEF += -D__ZX11_USE_PNG
endif

ifeq ($(CONFIG_USE_TIFF),y)
	LINK+=-ltiff
	DEF += -D__ZX11_USE_TIFF
endif

ifeq ($(CONFIG_USE_JPEG),y)
	LINK+=-ljpeg
	DEF += -D__ZX11_USE_JPEG
endif

ifeq ($(CONFIG_USE_DIB),y)
	DEF += -D__ZX11_USE_BMP
endif

ifeq ($(CONFIG_USE_MAG),y)
	DEF += -D__ZX11_USE_MAG
endif

ifeq ($(CONFIG_USE_PDT),y)
	DEF += -D__ZX11_USE_PDT
endif

LINK += -lX11 `zeda-config -l`
CFLAGS += $(DEF)

COMPILE=$(CC) $(CFLAGS) -o $@ $< $(LINK)

INSTALL=install -m 755
UNINSTALL=rm -f

CONFIGGEN=zeda-config-gen
CONFIG = zx11-config
TARGET=$(CONFIG) zxmagv zxpdtv

#####################################################################
all: $(TARGET)

$(CONFIG):
	@echo " GENERATE" $@
	-@rm -f $@
	@$(CONFIGGEN) -I $(PREFIX)/include -L $(PREFIX)/lib -D "$(DEF)" -l "$(LINK)" -v $(VERSION) > $@

%: %.c
	@echo " CC	" $<
	@$(COMPILE)
install: $(TARGET)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(TARGET)
uninstall:
	@echo " UNINSTALL	" $(TARGET)
	@cd $(BINDIR); $(UNINSTALL) $(TARGET); cd -
