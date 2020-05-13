# the source files to be built
SOURCES := main.cpp
MAIN := a.out

all:
	g++ $(SOURCES)

realclean:
	find . -type f -name "*.out" -exec rm '{}' \;

# this line required by make - don't delete
