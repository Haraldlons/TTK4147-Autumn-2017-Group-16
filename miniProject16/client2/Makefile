CC = /home/student/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc
# program executable name
TARGET = client
# compiler flags
CFLAGS = -g -Wall -D_GNU_SOURCE
# linker flags
LDFLAGS = -g -lrt -pthread
# list of SOURCES
SOURCES = $(shell find -name "*.c")
# default rule, to compile everything
all: $(TARGET)
# define object files
OBJECTS = $(SOURCES:.c=.o)
# link programs
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)
# compile
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)