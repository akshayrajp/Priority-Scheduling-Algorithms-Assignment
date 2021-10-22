CC = gcc
CFLAGS =
LDFLAGS = 
OBJFILES = helpers.o main.o ppsqa.o read_csv.o rrs.o
ifeq ($(OS),Windows_NT) 
    TARGET := a.exe
else
    TARGET := a.out
endif

all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET) *~