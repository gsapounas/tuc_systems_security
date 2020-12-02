CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic
OBJFILES = main.o rsa.o utils.o
LIBMATH = -lm

TARGET = assign_3


all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(DBG) -o $(TARGET) $(OBJFILES) $(LIBMATH)

clean:
	rm -f $(TARGET) *.o
