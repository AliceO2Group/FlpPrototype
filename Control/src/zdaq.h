/* 
 * File:   zdaq.h
 * Author: chapelan
 *
 * Created on July 25, 2013, 11:49 AM
 */

#ifndef ZDAQ_H
#define	ZDAQ_H


enum class mt_status {UNDEFINED, NOT_READY, READY, RUNNING, STOPPED, ERROR};
enum class mt_command {UNDEFINED, INIT, START, STOP, RELEASE, RESET};




#include "zdaq_ctrl.h"

class zdaqCtrl_config {
    std::string m_objectName;     // object name, as in zookeeper tree
    std::string m_DNS;            // path to Zookeeper server(s))
    
  public:  
    const char* getObjectName();
    const char *getDNS();
    
    int debug;
    
    zdaqCtrl_config(const char *objectName, const char* DNS);
    ~zdaqCtrl_config();
};




class daqModule: public zdaqCtrl_object {    
  
  ////////////////////////////////
  // implementation of zdaq_ctrl
  private:    
    int executeCommand(const char *command);
    
  /////////////////////////////////  
    
  /*
  mt_status getStatus();
  const char *getStatusByName();
  
  mt_status status;
 */
  
  
public:
  daqModule(zdaqCtrl_config);
  ~daqModule();
  
  protected:
  int setStatus(mt_status s);
 
  virtual int exec_INIT();
  virtual int exec_START();
  virtual int exec_STOP();
  virtual int exec_RELEASE();
  virtual int exec_RESET();
  virtual int do_loop(int maxItems);    // do_loop returns: -1 error, or the number of items processed (zero or positive).  Param: max number of items to process before returning (0: complete job until 1st error occur/finish).
  
private:
  pthread_t thread;  // thread for execution of loop
  int th_status;     // thread status: 0=stopped, 1=running, 2=error
  int th_do_stop;    // flag to stop thread loop
  protected:
  int th_stop_immediate;  // flag set to 1 when thread should complete immediately on STOP command, or if it should flush pending work first.
  pthread_t thread_id;
  
  public:
  void thread_loop();   // control main loop when running
  
  /*
  public:
  pthread_mutex_t   mutex_exec;   // mutex for execution of command
  pthread_t thread_cmd; // thread for asynch command exec
  mt_command exec_cmd;  // current command being executed
  int exec_status; // status of last command executed
 */ 
  // todo: check all variables initialized
  
  public:
  int stats_nItems_in;   // counter for number of items input by module
  int stats_nItems_out;  // counter for number of items output by module
  long long stats_nBytes_in;   // counter for number of bytes input by module
  long long stats_nBytes_out;  // counter for number of bytes output by module
};




class daqModule_fifo: public daqModule {
  public:
  daqModule_fifo(zdaqCtrl_config config,int size);
  ~daqModule_fifo();
  int write(void * data, int timeout);
  int read(void **, int timeout);
  // timeout in microseconds
  
  enum t_fifoAction {READ,WRITE};
  int setGlobalTimeout(int timeout, t_fifoAction a);  // set global timeout once for all next (READ|WRITE) actions with no timeout=0 specified. Allows to iterate several time while making sure total iterations not more than N seconds.


  int isFull();
  int isEmpty();
  
  private:
  int size;
  int index_start;
  int index_end;
  void **data;  
  int total_files;  /* a counter of the total number of files going through the FIFO */

  pthread_mutex_t   mutex;
  pthread_cond_t     cond;

  struct timespec gTimeoutValue[2];
  int gTimeoutActive[2];

  /* todo:
    option to set callback (free, delete, ...) to destroy each item still in the buffer when deleting FIFO
  */
};


class daqModule_producer: public daqModule {
  
  protected:
  daqModule_fifo *f_out;
  
  public:
  daqModule_producer(zdaqCtrl_config);
  ~daqModule_producer();
  
  int setFifoOut(daqModule_fifo *); // define the output of data producer
};

class daqModule_consumer: public daqModule {
  protected:
  daqModule_fifo *f_in;
  
  public:
  daqModule_consumer(zdaqCtrl_config);
  ~daqModule_consumer();
  
  int setFifoIn(daqModule_fifo *); // define the input of data consumer

};



class daqModule_producer_rand: public daqModule_producer {
  public:
  daqModule_producer_rand(zdaqCtrl_config);
  ~daqModule_producer_rand();
  
  int do_loop(int maxItems);
  
};



class daqModule_consumer_recordToFile: public daqModule_consumer {
  public:
  daqModule_consumer_recordToFile(zdaqCtrl_config);
  ~daqModule_consumer_recordToFile();
  
  int do_loop(int maxItems);
  int setFile(const char* f);

  int exec_INIT();
  int exec_RELEASE();

  private:
  char* filename;
  FILE *fd;
  int setup();
  int cleanup();

  unsigned long long nEvents;
  unsigned long long nBytes;
};


/* 
 * A dummy consumer module.
 * It just reads events in, and dumps them.
 */
class daqModule_consumer_dummy: public daqModule_consumer {
  public:
  daqModule_consumer_dummy(zdaqCtrl_config);
  ~daqModule_consumer_dummy(); 
  int do_loop(int maxItems);
};

/* 
 * A dummy producer module.
 * It just pushes a dummy (static) event.
 */
class daqModule_producer_dummy: public daqModule_producer {
  public:
  daqModule_producer_dummy(zdaqCtrl_config);
  ~daqModule_producer_dummy(); 
  int do_loop(int maxItems);
private:
    void* myEvent;
};





class daqModule_producer_netrx: public daqModule_producer {
  public:
  daqModule_producer_netrx(zdaqCtrl_config);
  ~daqModule_producer_netrx();
  
  int do_loop(int maxItems);

  int setPath(const char *);    // define 0mq connect/bind path
  
  int exec_INIT();
  int exec_RELEASE();

  private:
  int setup();
  int cleanup();
  
  void *zmq_context;
  void *zmq_responder;
  char *zmq_path;   // the 0mq path to connect/bind
  
  void *pendingEvent;
  
};


class daqModule_consumer_nettx: public daqModule_consumer {
  public:
  daqModule_consumer_nettx(zdaqCtrl_config);
  ~daqModule_consumer_nettx();
  
  int do_loop(int maxItems);

  int setPath(const char *);    // define 0mq connect/bind path
  
  int exec_INIT();
  int exec_RELEASE();

  private:
  int setup();
  int cleanup();
  

  void *zmq_context;
  void *zmq_requester;
  char *zmq_path;   // the 0mq path to connect/bind
};
#endif	/* ZDAQ_H */

