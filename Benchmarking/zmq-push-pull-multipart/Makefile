CC=g++
CXXFLAGS=-std=c++0x
LIBS=-lzmq

all: pull push clean

pull: Pull.o
	$(CC) $(CXXFLAGS) Pull.o -o Pull $(LIBS)

push: Push.o
	$(CC) $(CXXFLAGS) Push.o -o Push $(LIBS)

clean:
	rm -rf *.o

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $<
