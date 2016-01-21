CC=g++
CXXFLAGS=-std=c++0x
LIBS=-lpthread -lboost_system

flp: FLP.o
	$(CC) $(CXXFLAGS) FLP.o -o flp $(LIBS)

epn: EPN.o
	$(CC) $(CXXFLAGS) EPN.o -o epn $(LIBS) -lboost_thread

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $<