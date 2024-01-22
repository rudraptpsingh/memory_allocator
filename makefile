PROJECT = MemoryAllocator.exe

CC 			= g++
CVERSION 	= -std=c++20
CFLAGS 		= -Wall -Wextra -g3 $(CVERSION)

HEAPALLOCATOR		= heapallocator
BUMPALLOCATOR 		= bumpallocator
IMPLICITALLOCATOR 	= implicit
EXPLICITALLOCATOR	= explicit

TEST = test

SRCDIR		= src
INCDIR		= inc
OBJDIR		= obj
TARGETDIR	= bin
RESDIR		= res
TESTSDIR	= tests
INCEXT		= h
SRCEXT		= cpp
OBJEXT		= o
EXEEXT		= exe

# Define the header files
HDR	= $(HEAPALLOCATOR).$(INCEXT) $(BUMPALLOCATOR).$(INCEXT) $(IMPLICITALLOCATOR).$(INCEXT) $(EXPLICITALLOCATOR).$(INCEXT)

# Define the source files and object files
SRC	= $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJ = $(patsubst $(SRCDIR)/%.$(SRCEXT),$(OBJDIR)/%.$(OBJEXT),$(SRC))

all: $(TEST)

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(OBJDIR)

# Rule to build the object files from the source files and header files
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT) $(INCDIR)/%.$(INCEXT)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)

$(TEST): directories $(OBJ)
	$(CC) $(CFLAGS) $(TESTSDIR)/$(TEST).$(SRCEXT) -o $(TARGETDIR)/$(TEST).$(EXEEXT) $(OBJ)

.phony: clean directories

clean:
	rm -rf $(OBJDIR)
	rm -rf $(TARGETDIR)