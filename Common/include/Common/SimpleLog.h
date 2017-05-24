/// \file SimpleLog.h
/// \brief C++ interface implementing a simple logging format to a file
///
/// \author Sylvain Chapeland, CERN


#ifndef SRC_SIMPLE_LOG_H
#define SRC_SIMPLE_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <memory>

class SimpleLog {

  public:
  
  // constructor
  // \param logFilePath Path to log file. If NULL, using stdout/stderr.
  SimpleLog(const char* logFilePath=NULL);

  // destructor
  ~SimpleLog();
  
  // Set (or change) the log file name. Previous file used is closed. New file is appended when already existing.
  // \param logFilePath Path to log file. If NULL, using stdout/stderr.
  int setLogFile(const char* logFilePath=NULL);


  enum FormatOption : int {
    ShowTimeStamp = 0x1,
    ShowSeverityTxt = 0x2,
    ShowSeveritySymbol = 0x4,
    ShowMessage = 0x8
  };
  
  // Set output format based on (possibly OR-ed) format options from FormatOption enum
  void setOutputFormat(int opts);
  
  // Log an info message.
  // The message is formatted with timestamp and severity.
  // 
  // \param message Message, in a printf-like compatible format (with associated extra arguments)
  // NB: attribute format for printf-like argument check -> 1st (invisible) arg is 'this'
  int info(const char *message, ...) __attribute__ ((format (printf, 2,3)));

  // Log an error message. See info().
  int error(const char *message, ...) __attribute__ ((format (printf, 2,3)));
  
  // Log a warning message. See info().
  int warning(const char *message, ...) __attribute__ ((format (printf, 2,3)));
  
  
  // explicitely disable automatically generated methods
  // disable copy constructor
  SimpleLog(const SimpleLog&) =delete;
  // disable copy assignment operator
  SimpleLog& operator=(const SimpleLog&) =delete;
  
  private:
  class Impl;                       // private class for implementation
  std::unique_ptr<Impl> pImpl;      // handle to private class instance at runtime  
};

#endif /* SRC_SIMPLE_LOG_H */
