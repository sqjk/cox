###
# Standard changes of the Makefiles can be done here
DEBUG   := 1      ## debug mode if 1; release otherwise
MAKE_J  := -j4    ## if you want to build parallel
CCACHE  := ccache ## if you want to use ccache
SUBDIRS := $(wildcard qt gtk cox examples) ## subdirectories to build (if available)
                                           ## remove either if you don't want to build them


###
# If COX_LEVEL is not set, then we are the top-level Makefile. Otherwise, we
# are being included from a subdirectory makefile.

ifndef COX_LEVEL
  IS_TOP_LEVEL := 1
  COX_LEVEL    := .
endif

###
# Common Makefile code, shared by all cox Makefiles.
CXX      := $(CCACHE) g++
LIBS     += -lboost_signals
ifeq ($(EXT),)
EXT      := cc
endif
BUILDDIR := build
MAKE     := make $(MAKE_J)
AR       := ar

CXXFLAGS += -std=c++0x -I$(COX_LEVEL)
CXXFLAGS += -Wextra -Wall -Wno-long-long -pedantic-errors

ifeq ($(DEBUG),1)
  CXXFLAGS += -O0 -g
else
  CXXFLAGS += -Ofast -march=native
endif

override BUILDDIR := $(strip $(BUILDDIR))
SOURCES     := $(wildcard *.$(EXT))
OBJECTS     := $(patsubst %.$(EXT), $(BUILDDIR)/%.o, $(SOURCES))
EXAMPLES    := $(patsubst %.$(EXT), $(PREFIX)%, $(wildcard $(COX_LEVEL)/*.$(EXT)))
DEPS        := $(patsubst %.$(EXT), $(BUILDDIR)/%.dep, $(SOURCES))

ifeq ($(IS_TOP_LEVEL),1)

all: $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS): %:
	@$(MAKE) -C $@

.PHONY: clean
clean:
	@echo "   [ RM ] $(BUILDDIR)"
	@rm -rf $(BUILDDIR) 
	@$(MAKE) -C gtk clean
	@$(MAKE) -C qt  clean
	@$(MAKE) -C examples clean

else

ifneq ($(TARGET),)
all: $(TARGET)

$(TARGET): $(OBJECTS) $(DEPS)
	@echo "   [ AR ]  $@"
	@$(AR) rcs $(TARGET) $(OBJECTS)
else
all:  $(OBJECTS) $(DEPS)
endif

ifneq ($(MAKECMDGOALS), clean)
 -include $(DEPS)
endif

$(OBJECTS): $(BUILDDIR)/%.o: %.$(EXT) $(BUILDDIR)/%.dep $(BUILDDIR)/.tag
	@echo "   [ CC ]  $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

ifeq ($(PREBUILD),1)
$(DEPS): $(BUILDDIR)/%.dep: %.$(EXT) $(BUILDDIR)/.tag $(BUILDDIR)/%.$(PREBUILD_EXT)
else
$(DEPS): $(BUILDDIR)/%.dep: %.$(EXT) $(BUILDDIR)/.tag
endif
	@mkdir -p $(dir $(@))
	@echo "   [ DP ]  $<"
	@$(CXX) $(CXXFLAGS) -MM $< -MT $@ -MT $(<:.$(EXT)=.o) -o $@

%.tag:
	@mkdir -p $(dir $(@))
	@touch $@

.PHONY: clean
clean:
	@echo "   [ RM ] $(BUILDDIR)"
	@rm -rf $(BUILDDIR) 

endif
