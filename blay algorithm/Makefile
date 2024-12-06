CC = g++
CFLAGS = -Wall

COREDIR = core
ASMDIR = assembly

HEADERS = $(wildcard $(COREDIR)/*.h)

SOURCE = $(wildcard $(COREDIR)/*.cpp)

INCLUDES = -I $(COREDIR)

default: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o batch $(ASMDIR)/batch.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o automatic $(ASMDIR)/automatic.cpp $(SOURCE)
all: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o generator $(ASMDIR)/generator.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o solver $(ASMDIR)/solver.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o tester $(ASMDIR)/tester.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o batch $(ASMDIR)/batch.cpp $(SOURCE)
batch: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o batch $(ASMDIR)/batch.cpp $(SOURCE)
automatic: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o automatic $(ASMDIR)/automatic.cpp $(SOURCE)
clean:
	rm -f generator
	rm -f solver
	rm -f tester
	rm -f batch
	rm -f automatic