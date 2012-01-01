CXX      := g++
EXT      := cc
BUILDDIR := build
CXXFLAGS := -I.. -std=c++0x -Wextra -Wall -Wno-long-long -pedantic-errors

GOX_LIBS := -L../gtk -lcox -lboost_signals `pkg-config --libs gtk+-3.0`
QOX_LIBS := -L../qt  -lcox -lboost_signals `pkg-config --libs QtGui` 

override BUILDDIR := $(strip $(BUILDDIR))
SOURCES     := $(wildcard *.$(EXT))
OBJECTS     := $(patsubst %.$(EXT), $(BUILDDIR)/%.o, $(SOURCES))
GOX_TARGETS := $(patsubst %.$(EXT), gox-%, $(SOURCES))
QOX_TARGETS := $(patsubst %.$(EXT), qox-%, $(SOURCES))

all: $(GOX_TARGETS) $(QOX_TARGETS)

$(OBJECTS): $(BUILDDIR)/%.o: %.$(EXT) $(BUILDDIR)/.tag
	@echo "   [ CC ]  $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(GOX_TARGETS): gox-%: $(BUILDDIR)/%.o ../gtk/libcox.a
	@echo "   [ LN ]  $< (GTK+)"
	@$(CXX) $< -o $@ $(GOX_LIBS)

$(QOX_TARGETS): qox-%: $(BUILDDIR)/%.o ../qt/libcox.a
	@echo "   [ LN ]  $< (Qt)"
	@$(CXX) $< -o $@ $(QOX_LIBS)

%.tag:
	@mkdir -p $(dir $(@))
	@touch $@

.PHONY: clean
clean:
	@echo "   [ RM ] $(BUILDDIR)"
	@rm -rf $(BUILDDIR) 
