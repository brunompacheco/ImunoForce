CC = gcc
DOXYGEN = doxygen
SOURCEDIR = src
CFLAGS = -fdiagnostics-color=always -std=gnu11 -s -c -time
WARNINGS = -Wall
LIBRARIES = -lallegro -lallegro_image -lallegro_audio -lallegro_font -lallegro_primitives -lallegro_dialog -lallegro_acodec -lallegro_ttf -lm
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = imunoforce

all: 	CFLAGS += -DTYPE=ALL 	-O3
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBRARIES)

debug:	CFLAGS += -DTYPE=DEBUG	-g3
debug: $(EXECUTABLE)

.c.o:
	$(CC) $< -o $@ $(CFLAGS) $(WARNINGS)

.PHONY: doc clean
doc: 
	$(DOXYGEN) doxygen.config    
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)