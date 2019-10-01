include ../config

INCLUDE=-I. -I../include -I$(PREFIX)/include
LIB=-L../lib -L$(PREFIX)/lib
BINDIR=$(PREFIX)/bin

LINK=-lzx11

ifeq ($(CONFIG_USE_XEXT),y)
	LINK+=-lXext
endif

ifeq ($(CONFIG_USE_XPM),y)
	LINK+=-lXpm
endif

ifeq ($(CONFIG_USE_PNG),y)
	LINK+=-lpng -lz
endif

ifeq ($(CONFIG_USE_JPEG),y)
	LINK+=-ljpeg
endif

LINK+=-lX11 `zeda-config -l`

INSTALL=install -m 755
UNINSTALL=rm -f

CONFIGGEN=zeda-config-gen
CONFIG = zx11-config
TARGET=$(CONFIG)

#####################################################################
all: $(TARGET)

$(CONFIG):
	@echo " GENERATE" $@
	-@rm -f $@
	@$(CONFIGGEN) -I $(PREFIX)/include -L $(PREFIX)/lib -l "$(LINK)" -v $(VERSION) > $@

install: $(TARGET)
	@echo " INSTALL	" $^
	@$(INSTALL) $^ $(BINDIR)
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ core $(TARGET)
uninstall:
	@echo " UNINSTALL	" $(TARGET)
	@cd $(BINDIR); $(UNINSTALL) $(TARGET); cd -
