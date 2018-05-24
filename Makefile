CC := g++

BINDIR := bin
SRCDIR := src
BUILDDIR := build
TARGET := $(BINDIR)/server

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -lpthread -g
INC := -I include

$(TARGET) : $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $(TARGET) $(CFLAGS)

$(BUILDDIR)/%.o : $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: clean
