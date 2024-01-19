PROJECT = MemoryAllocator.exe

CC = g++
CFLAGS = -Wall -Wextra -g3
CVERSION = -std=c++20

BUMPALLOCATOR = bumpallocator

TEST = test

SRCDIR		= src
INCDIR		= inc
BUILDDIR	= obj
TARGETDIR	= bin
RESDIR		= res
SRCEXT		= cpp
OBJEXT		= o

all: resources $(BUMPALLOCATOR) $(TEST)

resources: directories
#	@cp $(RESDIR)/* $(TARGETDIR)

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

$(BUMPALLOCATOR): directories
	$(CC) $(CFLAGS) -c $(CVERSION) $(SRCDIR)/$(BUMPALLOCATOR).cpp -o $(BUILDDIR)/$(BUMPALLOCATOR).o 

$(TEST): resources $(BUILDDIR)/$(BUMPALLOCATOR).o
	$(CC) $(CFLAGS) $(CVERSION) $(SRCDIR)/$(TEST).cpp -o $(BUILDDIR)/$(TEST).o $(BUILDDIR)/$(BUMPALLOCATOR).o

.phony: all clean directories

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGETDIR)