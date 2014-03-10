CC := g++
CFLAGS := -g -O3  -Wall #-v  
ALLDEPS := 
SRCDIR := src
BUILDDIR := build

SRCEXT := cpp
HDRPAT := -name *.h
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
LIBDIR := /usr/local/include
HEADERS := $(shell find $(SRCDIR) -type f $(HDRPAT))
ALLDEP += $(HEADERS)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LIB := -lm
FW := -framework OpenCL
INC := -I include -I$(LIBDIR)

all: $(OBJECTS)
	@echo "$(CC) $(CFLAGS) $(INC) -o build/test $(OBJECTS)"; $(CC) $(CFLAGS)  $(FW) $(INC) -o build/out $(OBJECTS)

$(BUILDDIR)/%.o:	$(SRCDIR)/%.$(SRCEXT) $(ALLDEP)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm build/*.o
