COX_LEVEL := ..
PREFIX    := qox-
CXXFLAGS  := `pkg-config --cflags QtGui` -fPIC
LIBS      := `pkg-config --libs QtGui`
TARGET    := libcox.a

MOC       := $(CCACHE) moc

PREBUILD     := 1
PREBUILD_EXT := moc

include $(COX_LEVEL)/Makefile

MOCFILES := $(patsubst %.$(EXT), $(BUILDDIR)/%.moc, $(SOURCES))

$(MOCFILES): $(BUILDDIR)/%.moc: %.$(EXT) $(BUILDDIR)/.tag
	@echo "   [MOC ]  $<"
	@$(MOC) -i $< -o $@
