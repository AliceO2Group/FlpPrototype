///
/// \file   signalUtilities.h
/// \author Barthelemy von Haller
/// \brief  This file proposes a set of functions to handle the signals in your program.
///
/// Typically, you want to print the stack trace when there is a sigsev.
/// It also handles elegantly SIGINT and SIGTERM by catching them and allowing the program
/// to clean itself up before exiting (see handler_interruption). If a second SIGINT/SIGTERM is
/// received before the end of the clean up we exit immediately (return value 0).
///

#ifndef COMMON_SIGNALUTILITIES_H
#define COMMON_SIGNALUTILITIES_H

#include <execinfo.h>
#include <csignal>

/// \brief Print all the entries of the stack on stderr
/// \author Barthelemy von Haller
void printStack()
{
  void *array[10];
  int size;
  size = backtrace(array, 10);
  backtrace_symbols_fd(array, size, 2);
}

/// \brief Callback for SigSev that will print the stack before returning 1 and exiting.
/// Usage : signal(SIGSEGV, handler_sigsev);
/// \author Barthelemy von Haller
void handler_sigsev(int sig)
{
  fprintf(stderr, "Error: signal %d:\n", sig);
  std::cerr << "Error: signal " << sig << "\n";
  printStack();
  exit(1);
}

bool keepRunning = true; /// Indicates whether we should continue the execution loop or not.
std::string handler_interruption_message = "Catched signal 2 \nExit the process at the end of this cycle. "
  "\n  Press again Ctrl-C to force immediate exit";

/// \brief Callback for interruption signals such as SIGINT and SIGTERM allowing the program to clean itself up.
/// The variable #keepRunning is available to know that we have not been interrupted. In case it becomes false,
/// one should start cleaning up before exiting. If a second SIGINT/SIGTERM is
/// received before the end of the clean up we exit immediately (return value 0).
/// Usage :
/// \code
///    #include <csignal>
///    signal(SIGINT, handler_interruption);
///    while (keepRunning) {
///      // do something
///    }
///    // clean up
///    return EXIT_SUCCESS;
/// \endcode
/// \author Barthelemy von Haller
void handler_interruption(int sig)
{
  if (keepRunning) {
    std::cout << handler_interruption_message << std::endl;
    keepRunning = false;
  } else {
    std::cout << "Second interruption : immediate exit" << std::endl;
    exit(0);
  }
}

#endif //COMMON_SIGNALUTILITIES_H
