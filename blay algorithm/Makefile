CC = g++
CFLAGS = -Wall
DLLFLAGS = -fpic -Wall -shared

COREDIR = core
ASMDIR = assembly
DLLDIR = dll

HEADERS = $(wildcard $(COREDIR)/*.h)

SOURCE = $(wildcard $(COREDIR)/*.cpp)

DLLHEADERS = $(wildcard $(DLLDIR)/*.h)

DLLSOURCE = $(wildcard $(DLLDIR)/*.cpp)

INCLUDES = -I $(COREDIR)

all: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o auto $(ASMDIR)/auto.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o manual $(ASMDIR)/manual.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o website $(ASMDIR)/website.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o generator $(ASMDIR)/generator.cpp $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) -o tester $(ASMDIR)/tester.cpp $(SOURCE)
automatic: auto
auto: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o auto $(ASMDIR)/auto.cpp $(SOURCE)
manual:
	$(CC) $(CFLAGS) $(INCLUDES) -o manual $(ASMDIR)/manual.cpp $(SOURCE)
website: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o website $(ASMDIR)/website.cpp $(SOURCE)
generator:
	$(CC) $(CFLAGS) $(INCLUDES) -o generator $(ASMDIR)/generator.cpp $(SOURCE)
tester: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -o tester $(ASMDIR)/tester.cpp $(SOURCE)
dll: dll64
dll64: $(SOURCE) $(HEADERS) $(DLLSOURCE) $(DLLHEADERS)
	x86_64-w64-mingw32-g++ $(DLLFLAGS) -m64 $(INCLUDES) -o RPG.dll $(DLLSOURCE) $(SOURCE)
dll32: $(SOURCE) $(HEADERS) $(DLLSOURCE) $(DLLHEADERS)
	i686-w64-mingw32-g++ $(DLLFLAGS) -m32 $(INCLUDES) -o RPG32.dll $(DLLSOURCE) $(SOURCE)
clean:
	rm -f auto
	rm -f manual
	rm -f website
	rm -f dailyPuzzle.txt
	rm -f generator
	rm -f tester
	rm -f RPG.dll
	rm -f RPG32.dll