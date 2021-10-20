CC = gcc
CFLAGS =
LDFLAGS = 
OBJFILES = helpers.o main.o ppsqa.o read_csv.o rrs.o 
TARGET = a.out

all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~