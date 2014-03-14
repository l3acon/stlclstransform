CC := g++
CFLAGS := -g -O3  -Wall #-v  
ALLDEPS := 
SRCDIR := src
BUILDDIR := build

# edit this for even more portablility 
UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        FW := -lOpenCL
        LIBDIR :=/opt/AMDAPP/include/
        LIB := -lm -lrt
    endif
    ifeq ($(UNAME_S),Darwin)
        FW := -framework OpenCL
        LIBDIR := /usr/local/include
        LIB := -lm
    endif

SRCEXT := cpp
HDRPAT := -name *.h
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
HEADERS := $(shell find $(SRCDIR) -type f $(HDRPAT))
ALLDEP += $(HEADERS)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC := -I include -I$(LIBDIR)

all: $(OBJECTS)
	@echo "$(CC) $(CFLAGS) $(INC) -o build/test $(OBJECTS)"; $(CC) $(CFLAGS) $(LIB) $(FW) $(INC) -o build/out $(OBJECTS)

$(BUILDDIR)/%.o:	$(SRCDIR)/%.$(SRCEXT) $(ALLDEP)
	@mkdir -p $(BUILDDIR)
	@echo "$(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm build/*.o
