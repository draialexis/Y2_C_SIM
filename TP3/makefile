CC=gcc

CFLAGS=-c -Wall
LDFLAGS=-lm
SOURCES=main.c mt.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=prog

.PHONY: clean
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
