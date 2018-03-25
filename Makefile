#directory
PBERRDIR=../PBErr
GSETDIR=../GSet
PBMATHDIR=../PBMath

# Build mode
# 0: development (max safety, no optimisation)
# 1: release (min safety, optimisation)
# 2: fast and furious (no safety, optimisation)
BUILDMODE=1

include $(PBERRDIR)/Makefile.inc

INCPATH=-I./ -I$(PBERRDIR)/ -I$(GSETDIR)/ -I$(PBMATHDIR)/
BUILDOPTIONS=$(BUILDPARAM) $(INCPATH)

# compiler
COMPILER=gcc

#rules
all : main

main: main.o pberr.o grad.o gset.o pbmath.o Makefile 
	$(COMPILER) main.o pberr.o grad.o gset.o pbmath.o $(LINKOPTIONS) -o main

main.o : main.c $(PBERRDIR)/pberr.h grad.h grad-inline.c Makefile
	$(COMPILER) $(BUILDOPTIONS) -c main.c

grad.o : grad.c grad.h $(GSETDIR)/gset.h $(PBMATHDIR)/pbmath.h grad-inline.c Makefile
	$(COMPILER) $(BUILDOPTIONS) -c grad.c

gset.o : $(GSETDIR)/gset.c $(GSETDIR)/gset-inline.c $(GSETDIR)/gset.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(GSETDIR)/gset.c

pbmath.o : $(PBMATHDIR)/pbmath.c $(PBMATHDIR)/pbmath-inline.c $(PBMATHDIR)/pbmath.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(PBMATHDIR)/pbmath.c

pberr.o : $(PBERRDIR)/pberr.c $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(PBERRDIR)/pberr.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main
	
unitTest :
	main > unitTest.txt; diff unitTest.txt unitTestRef.txt
