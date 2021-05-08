include ../config

BINDIR=$(PREFIX)/bin

INSTALL=install -m 755
UNINSTALL=rm -f

INCLUDE=`zeda-config -I`
LIB=`zeda-config -L`
DEF=`zeda-config -D`

LINK=-lzx11

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

CONFIGGEN=`which zeda-config-gen`
CONFIG=zx11-config

#####################################################################
all: $(CONFIG)

$(CONFIG):
	@echo " GENERATE	" $@
	-@rm -f $@
	@$(CONFIGGEN) -I "$(INCLUDE)" -L "$(LIB)" -D "$(DEF)" -l "$(LINK)" -v $(VERSION) > $@
install: $(CONFIG)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(CONFIG)
uninstall:
	@echo " UNINSTALL	" $(CONFIG)
	@$(UNINSTALL) $(BINDIR)/$(CONFIG)
