TARGET = punyml
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

SRCDIR = src
INCDIR = $(SRCDIR)/inc
OBJDIR = obj

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
HEADERS = $(wildcard $(INCDIR)/*.h)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f obj/*.o
	-rm -f $(TARGET)