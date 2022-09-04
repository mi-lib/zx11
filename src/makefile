MAKEFILEGEN=`which zeda-makefile-gen`

all:
	@$(MAKEFILEGEN) lib | make -f - STD=
clean:
	@$(MAKEFILEGEN) lib | make -f - clean
