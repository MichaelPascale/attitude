CC = cc
CFLAGS = -Wall -pedantic -std=c89
OBJECTS = file.o timecvt.o
EXE = tt

all: tt

tt: $(OBJECTS)
	$(CC) -lm $(CFLAGS) -o $(EXE) main.c $(OBJECTS)

file.o: file.c file.h
	$(CC) $(CFLAGS) -c file.c

timecvt.o: timecvt.c timecvt.h
	$(CC) $(CFLAGS) -c timecvt.c

clean:
	rm $(OBJECTS) $(EXE)