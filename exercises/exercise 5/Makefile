# program executable name
TARGET = main_fx
# compiler flags
CFLAGS = -g -Wall -D_GNU_SOURCE
# linker flags
LDFLAGS = -g -lrt -pthread -lcomedi -lm
# list of SOURCES
SOURCES = $(shell find -name "*.c")
# default rule, to compile everything
all: $(TARGET)
# define object files
OBJECTS = $(SOURCES:.c=.o)
# link programs
$(TARGET): $(OBJECTS)
	gcc  -o $@ $^ $(LDFLAGS)
# compile
%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<
# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
