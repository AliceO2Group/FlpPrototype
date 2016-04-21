CC=g++
CXXFLAGS=-std=c++0x
LIBS=-L/home/alfa/FairSoft_install/lib -lpthread -lboost_system 
INC=-I/home/alfa/FairSoft_install/include/

all: flp epn
flp: FLP.o
	$(CC) $(CXXFLAGS) FLP.o -o flp $(LIBS)

epn: EPN.o
	$(CC) $(CXXFLAGS) EPN.o -o epn $(LIBS) -lboost_thread

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< $(INC)
