MAKEFILEGEN=`which zeda-makefile-gen`

all:
	@$(MAKEFILEGEN) lib-no-ansi | make -f -
clean:
	@$(MAKEFILEGEN) lib-no-ansi | make -f - clean
