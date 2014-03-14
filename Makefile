SHELL = /bin/ksh

CC := g++
CFLAGS := -g -O3  -Wall #-v  
ALLDEPS := 
SRCDIR := src
BUILDDIR := build

#these might not necessarily be right
AMDAPPDIR :=/opt/AMDAPP
NVIDIASDKDIR :=/usr/local/cuda
INTELSDKDIR :=/usr/lib64/OpenCL/vendors/intel

# assume there's only one SDK installed
VENDOR_S := $(shell [ -d $(AMDAPPDIR) ] && echo "AMD")
VENDOR_S := $(shell [ -d $(NVIDIASDKDIR) ] && echo "NVIDIA")
VENDOR_S := $(shell [ -d $(INTELSDKDIR) ] && echo "INTEL")

# using uname to deterime operating system
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ifeq ($(VENDOR_S),AMD)
	LIBDIR :=$(AMDAPPDIR)
	endif
	ifeq ($(VENDOR_S),NVIDIA)
	LIBDIR :=$(NVIDIASDKDIR)
	endif
	ifeq ($(VENDOR_S),INTEL)
	LIBDIR :=$(INTELSDKDIR)
	endif
	FW := -lOpenCL
	LIB := -lm -lrt
endif

#assume this works for all macs
ifeq ($(UNAME_S),Darwin)
    FW := -framework OpenCL
    LIBDIR := /usr/local/include
    LIB := -lm	#mac doesn't have "linux real time"
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
