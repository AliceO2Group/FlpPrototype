# Common module

This module contains reusable code shared between other modules,
and provides various features.
Here is a list and short description of the header files.


### Configuration.h

Class to retrieve configuration parameters (key/value pairs) from a text file (e.g. .ini format).
This feature is also exported as a Tcl library, libtclConfiguration.so

### Daemon.h

Class to help creating a daemon process running in the background.

### Exceptions.h

### Fifo.h

Class implementing  a lock-free 1-to-1 FIFO.
Used to pass data between threads.

### LineBuffer.h

Class implementing a buffer to read from file descriptor and get out data line by line.

### MakeUnique.h

Provides std::make_unique to c++11. Superseded by C++14, do not use.

### signalUtilities.h

### SimpleLog.h

Class providing simple logging format to a file (or standard output).

### Thread.h

Class to implement controllable looping threads.

### Timer.h

Class to implement a high resolution timer function.
(counting time or timeout)
