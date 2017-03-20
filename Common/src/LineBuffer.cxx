///
/// \file    LineBuffer.cxx
/// \brief   Implementation of LineBuffer class. See LineBuffer.h.
/// \author  Sylvain Chapeland
///

#include <Common/LineBuffer.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

LineBuffer::LineBuffer() {
}

LineBuffer::~LineBuffer() {
}

int LineBuffer::appendFromFileDescriptor(int fd, int timeout) {
  fd_set rfds;
  int ret;
  struct timeval tv;
  
  const int bufferSize=1000;  // size of chunks read from file descriptor
  char buffer[bufferSize];
  
  for(;;) {

    // wait new data until timeout, if any
    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);
    if (timeout>=0) {
      tv.tv_sec=timeout/1000;
      tv.tv_usec=(timeout-tv.tv_sec*1000)*1000;
      ret=select(fd+1,&rfds,NULL,NULL,&tv);
    } else {
      ret=select(fd+1,&rfds,NULL,NULL,NULL);
    }   
    if (ret==-1) {
      break;
    }
    
    if (FD_ISSET(fd,&rfds)) {
      // read data (keep a spare char in buffer for zero-termination)
      ret=read(fd,buffer,bufferSize-1);

      // process new chars
      if (ret>0) {
        // add zero-termination
        buffer[ret]=0;
 
        // parse and extract lines from buffer
        char *ptr=buffer;
        while (*ptr!=0) {
          char *endline;
          // where's the next end of line?
          endline=strchr(ptr,'\n');
          if (endline==NULL) {
            // not found, keep pending chars for next round
            pendingLine.append(ptr);
            break;
          }
          // add separator at end of line
          *endline=0;
          // append to previous pending buffer
          pendingLine.append(ptr);
          // append to vector of completed lines
          completeLines.push(pendingLine);
          pendingLine.clear();
          // iterate with next part
          ptr=++endline;
        }
      } else if (ret==0) {
        return -1;
      } else {
        break;
      }
    } else {
      break;
    }
    
    // continue to iterate reading from fd, but don't wait if nothing left
    timeout=0;
  }
  return 0;
}


int LineBuffer::getNextLine(std::string &nextLine) {
  if (completeLines.empty()) {
    // no complete line yet
    return -1;
  }
  // return next line from buffer (by reference)
  nextLine=completeLines.front();
  completeLines.pop();
  return 0;
}


//void LineBuffer::appendString(const char *s) {}
