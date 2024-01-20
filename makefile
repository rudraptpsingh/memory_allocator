PROJECT = MemoryAllocator.exe

CC 			= g++
CFLAGS 		= -Wall -Wextra -g3
CVERSION 	= -std=c++20

BUMPALLOCATOR 		= bumpallocator
IMPLICITALLOCATOR 	= implicit

TEST = test

SRCDIR		= src
INCDIR		= inc
BUILDDIR	= obj
TARGETDIR	= bin
RESDIR		= res
SRCEXT		= cpp
OBJEXT		= o

all: $(TEST)

resources: directories
#	@cp $(RESDIR)/* $(TARGETDIR)

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

$(BUMPALLOCATOR):
	$(CC) $(CFLAGS) -c $(CVERSION) $(SRCDIR)/$(BUMPALLOCATOR).cpp -o $(BUILDDIR)/$(BUMPALLOCATOR).o 

$(IMPLICITALLOCATOR):
	$(CC) $(CFLAGS) -c $(CVERSION) $(SRCDIR)/$(IMPLICITALLOCATOR).cpp -o $(BUILDDIR)/$(IMPLICITALLOCATOR).o 

$(TEST): resources $(BUMPALLOCATOR) $(IMPLICITALLOCATOR)
	$(CC) $(CFLAGS) $(CVERSION) $(SRCDIR)/$(TEST).cpp -o $(BUILDDIR)/$(TEST).o $(BUILDDIR)/$(BUMPALLOCATOR).o $(BUILDDIR)/$(IMPLICITALLOCATOR).o

.phony: all clean directories

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGETDIR)