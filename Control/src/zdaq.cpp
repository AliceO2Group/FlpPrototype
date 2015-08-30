#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "zdaq.h"

using namespace std;


zdaqCtrl_config::zdaqCtrl_config(const char *objectName, const char* DNS) {
     m_objectName=objectName;
     m_DNS=DNS;
     
     debug=0;
 }

 zdaqCtrl_config::~zdaqCtrl_config() {
 }

const char* zdaqCtrl_config:: getObjectName() {
     return m_objectName.c_str();
 }
    
const char* zdaqCtrl_config:: getDNS() {
    return m_DNS.c_str();
}



daqModule::daqModule(zdaqCtrl_config c): zdaqCtrl_object(c.getObjectName(),c.getDNS())  {
  th_status=0;
  th_do_stop=0;
  th_stop_immediate=1;
  thread_id=0;
  
  stats_nItems_in=0;
  stats_nItems_out=0;  
  stats_nBytes_in=0;
  stats_nBytes_out=0;  
  setStatus(mt_status::NOT_READY);
}

daqModule::~daqModule() {
  //cout << "destroy " << getName() << endl;
  fflush(stdout);
  if (th_status) {
    cout << "desctructor: waiting thread exit" << endl;
    th_stop_immediate=1;
    th_do_stop=1;
    int i;
    for(i=0;i<1000;i++) {
        if (!th_status) {break;}
        usleep(1000);
    }
    if (th_status) {
        cout << "canceling thread" << endl;
        pthread_cancel(thread);
    }
    pthread_join(thread,NULL); // todo: check thread exit code
    cout << "thread completed" << endl;
  }
  
  
  cout << getName() << " : " << stats_nItems_in << " items in" << endl;
  cout << getName() << " : " << stats_nItems_out << " items out" << endl;
  cout << getName() << " : " << stats_nBytes_in << " bytes in" << endl;
  cout << getName() << " : " << stats_nBytes_out << " bytes out" << endl;
  
}


void *daqModule_loop(void *arg) {
  daqModule *m;
  m=(daqModule *)arg;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
  m->thread_loop();
  return 0;
}

mt_status getStatusFromString(const char *c) {
    if (c==NULL) {return mt_status::UNDEFINED;}    
    else if (!strcmp(c,"NOT_READY")) {return mt_status::NOT_READY;}
    else if (!strcmp(c,"READY")) {return mt_status::READY;}
    else if (!strcmp(c,"RUNNING")) {return mt_status::RUNNING;}
    else if (!strcmp(c,"STOPPED")) {return mt_status::STOPPED;}
    else if (!strcmp(c,"ERROR")) {return mt_status::ERROR;}
    
    return mt_status::UNDEFINED;
}

mt_command getCommandFromString(const char *c) {
    if (c==NULL) {return mt_command::UNDEFINED;}
    else if (!strcmp(c,"INIT")) {return mt_command::INIT;}
    else if (!strcmp(c,"START")) {return mt_command::START;}
    else if (!strcmp(c,"STOP")) {return mt_command::STOP;}
    else if (!strcmp(c,"RELEASE")) {return mt_command::RELEASE;}
    else if (!strcmp(c,"RESET")) {return mt_command::RESET;}

    return mt_command::UNDEFINED;
}

const char *getStringFromStatus( mt_status m) {
    if (m==mt_status::NOT_READY) { return "NOT_READY"; }
    else if (m==mt_status::READY) { return "READY"; }
    else if (m==mt_status::RUNNING) { return "RUNNING"; }
    else if (m==mt_status::STOPPED) { return "STOPPED"; }
    else if (m==mt_status::ERROR) { return "ERROR"; }
    return "UNDEFINED";
}
int daqModule::setStatus(mt_status s) {
    return setState(getStringFromStatus(s));
}
int daqModule::executeCommand(const char *command) {

  // this is called by control loop, beware of sync with loop thread
  int success=0;
  int error=0;

  //cout << getName() << " executing command " << command << endl;
  
  mt_command m;
  m=getCommandFromString(command);
  
  mt_status currentStatus=mt_status::UNDEFINED;
  mt_status newStatus=mt_status::UNDEFINED;
  
  currentStatus=getStatusFromString(this->zdaqCtrl_object::getState());
  newStatus=currentStatus;
  
  //cout << getName() << " current status " << getStringFromStatus(currentStatus) << endl;
  
  switch (m) {
    case mt_command::INIT:
      if (currentStatus==mt_status::NOT_READY) {
        if (exec_INIT()==0) {
          newStatus=mt_status::READY;
          success=1;
        } else {
          error=1;
        }
      }
      break;
    case mt_command::START:
      if (currentStatus==mt_status::READY) {
        if (pthread_create(&thread,NULL,&daqModule_loop,(void *) this)) {
          error=1;
          break;
        }      
        stats_nItems_in=0;
        stats_nItems_out=0;
        stats_nBytes_in=0;
        stats_nBytes_out=0;
        if (exec_START()==0) {
          newStatus=mt_status::RUNNING;
          success=1;
        } else {
          error=1;
        }
      }
      break;
    case mt_command::STOP:
      if (currentStatus==mt_status::RUNNING) {
        th_do_stop=1;
        while (th_status==1) {
          // wait thread to complete
          //cout << "wait thread to complete" << endl;
          usleep(1000);
          // todo: add timeout
        }
        pthread_join(thread,NULL); // todo: check thread exit code
        if (exec_STOP()==0) {
          newStatus=mt_status::STOPPED;
          success=1;
        } else {
          error=1;
        }
      }
      break;
    case mt_command::RELEASE:
      if ((currentStatus==mt_status::READY)||(currentStatus==mt_status::STOPPED)) {
        if (exec_RELEASE()==0) {
          newStatus=mt_status::NOT_READY;
          success=1;
        } else {
          error=1;
        }
      }
      break;      
   case mt_command::RESET:
      if (currentStatus==mt_status::ERROR) {
        if (exec_RESET()==0) {
          newStatus=mt_status::NOT_READY;
          success=1;
        } else {
          error=1;
        }
      }
      break;
    case mt_command::UNDEFINED:
      // TODO
      break;
  }
  if (error) {
    // go to state error if execution failed
    // but not if wrong command received
    newStatus=mt_status::ERROR;
  }

  setStatus(newStatus);
  //cout << getName() << " after command " << (int)m << " status= " << getStringFromStatus(newStatus) << endl;
  return success;
}



int daqModule::exec_INIT() {
  return 0;
}
int daqModule::exec_START(){
  return 0;
}
int daqModule::exec_STOP(){
  return 0;
}
int daqModule::exec_RELEASE(){
  return 0;
}
int daqModule:: exec_RESET(){
  return 0;
}
int daqModule::do_loop(int maxItems){
  usleep(1000);
  return 0;
}


void daqModule::thread_loop() {
  thread_id=pthread_self();
  //cout << "thread " << thread_id << " starting" << endl;
  th_status=1;
  while (!th_do_stop) {  
//    cout << "loop tick " << endl;
    if (this->do_loop(100)<0) {
      th_status=2;
      break;
    }
  }

  // flush thread work if configured to do so
  //cout << "stop_immediate = " << th_stop_immediate << endl;
  if (!th_stop_immediate) {
    //cout << "thread " << thread_id << " stopping" << endl;
    if (this->do_loop(0)<0) {
      th_status=2;
    }
  }

  //cout << "thread " << thread_id << " ended" << endl;
  th_status=0;
  return;
}



/* add a number of microseconds to a timeval struct */
void timevalAddUsec(struct timeval *t, int microseconds) {
  t->tv_usec+=microseconds % 1000000;
  int s;
  if (t->tv_usec>=1000000) {
    t->tv_usec-=1000000;
    t->tv_sec++;
  }
  t->tv_sec+=microseconds/1000000;
}





/* FIFO ********************************/





/* todo:
   this is the implementation optimized for 1-1 operation (1 writer, 1 reader)
   lock-free unless wait needed
*/

daqModule_fifo::daqModule_fifo(zdaqCtrl_config c,int size):daqModule(c) {
  this->size=size;
  this->data=new void* [size+1];
/*  this->data=(void **)malloc(sizeof(void *)*(size+1));
  if (this->data==NULL) {throw "malloc() failed";}
*/
  int i;
  for(i=0;i<size;i++) {
    this->data[i]=NULL;
  }
  this->index_start=0;
  this->index_end=0;
  this->total_files=0;
  
  for (i=0;i<2;i++) {
    this->gTimeoutActive[i]=0;
  }
  
  pthread_mutex_init(&this->mutex,NULL);
  pthread_cond_init(&this->cond,NULL);
  //setStatus(mt_status::READY);
};
int daqModule_fifo::setGlobalTimeout(int timeout,t_fifoAction a) {
  if (timeout==0) {
    gTimeoutActive[a]=0;
  } else {
    gTimeoutActive[a]=1;
    struct timeval now;
    gettimeofday(&now,NULL);
    timevalAddUsec(&now,timeout);
    gTimeoutValue[a].tv_sec = now.tv_sec;
    gTimeoutValue[a].tv_nsec = now.tv_usec * 1000;
  }
  return 0;
}


daqModule_fifo::~daqModule_fifo() {
  cout << "deleting FIFO" << endl;
  
  /* if not empty, warning */
  if (this->index_start!=this->index_end) {
    cout << "FIFO_destroy: not empty, possible memory leak" << endl;
  }
  pthread_mutex_lock(&this->mutex);
  pthread_cond_destroy(&this->cond);
  pthread_mutex_destroy(&this->mutex);

  delete[] this->data;
  
/*  if (this->data!=NULL) {
    free(this->data);
  }
*/
}

// check if there is an item available to be read
int daqModule_fifo::isEmpty() {
  if (this->index_start==this->index_end) {
      return 0;
  }
  
  /* nothing in the FIFO */  
  return 1;

}

// check if there is an empty slot to write to
int daqModule_fifo::isFull() {
    int new_index_end;
    
    /* new end buffer position */
    new_index_end=this->index_end+1;
    if (new_index_end>this->size) {
      new_index_end=0;
    }

    /* can append new item only if space left */
    if (new_index_end!=this->index_start) {
      return 0;
    }
    return 1;
}


/* implementation mostly lock free (locks only on blocking calls)
   assumes only 1 reader and 1 writer at a time.
*/


int daqModule_fifo::write(void *item, int timeout) {
  int new_index_end;
  struct timeval now;
  struct timespec t;
  int    t_set=0;
  int    retcode;

  if (item==NULL) return -1;
  if (this->data==NULL) return -1;
  
  /* wait the FIFO has a free slot */
  for(;;) {
    /* new end buffer position */
    new_index_end=this->index_end+1;
    if (new_index_end>this->size) {
      new_index_end=0;
    }

    /* append new item only if space left */
    if (new_index_end!=this->index_start) {
      break;
    }
    
    /* no space in FIFO */  
    if (timeout==0) {
      /* no wait requested, return */
      return -1;      
      
    } else if (timeout>0) {
      /* timeout defined */
      if (!t_set) {
        gettimeofday(&now,NULL);
        timevalAddUsec(&now,timeout);
        t.tv_sec = now.tv_sec;
        t.tv_nsec = now.tv_usec * 1000;
        t_set=1;
      }

      pthread_mutex_lock(&this->mutex);
      retcode = pthread_cond_timedwait(&this->cond, &this->mutex, &t);
      pthread_mutex_unlock(&this->mutex);
      if (retcode==ETIMEDOUT) {
        /* timeout : return */
        return -1;      
      }

    } else {
      pthread_mutex_lock(&this->mutex);
      /* wait forever */
      pthread_cond_wait(&this->cond, &this->mutex);
      pthread_mutex_unlock(&this->mutex);
    }      
  }
//  cout << this->index_end << endl;
  this->data[this->index_end]=item;
  this->index_end=new_index_end;
  stats_nItems_in++;

  /* notify FIFO update */
//  pthread_mutex_lock(&this->mutex);
  pthread_cond_signal(&this->cond);
//  pthread_mutex_unlock(&this->mutex);
 
  return 0;
}


int daqModule_fifo::read(void **item, int timeout) {
  void *ret_val;
  struct timeval now;
  struct timespec t;
  int t_set=0;
  int retcode;
  int new_index_start;
  
  if (item==NULL) return -1;
  *item=NULL;
  if (this->data==NULL) return -1;

  /* wait for the FIFO to have something in */
  while (this->index_start==this->index_end) {

    /* nothing in the FIFO */  

    if (timeout==0) {
      /* no wait requested, return */
      return -1;      
      
    } else if (timeout>0) {
      /* timeout defined */
      if (!t_set) {
        gettimeofday(&now,NULL);
        timevalAddUsec(&now,timeout);
        t.tv_sec = now.tv_sec;
        t.tv_nsec = now.tv_usec * 1000;
        t_set=1;
      }

      pthread_mutex_lock(&this->mutex);
      retcode = pthread_cond_timedwait(&this->cond, &this->mutex, &t);
      pthread_mutex_unlock(&this->mutex);
       
      if (retcode==ETIMEDOUT) {
        /* timeout : return */       
        return -1;      
      }

    } else {  
      /* wait forever */
      pthread_mutex_lock(&this->mutex);
      pthread_cond_wait(&this->cond, &this->mutex);  
      pthread_mutex_unlock(&this->mutex);
    }
  }

  /* get the first item from FIFO */
  *item=this->data[this->index_start];
  this->data[this->index_start]=NULL;
  new_index_start=this->index_start+1;
  if (new_index_start>this->size) {
    new_index_start=0;
  }
  this->index_start=new_index_start;
  stats_nItems_out++;

  /* notify FIFO update */
//  pthread_mutex_lock(&this->mutex);
  pthread_cond_signal(&this->cond);
//  pthread_mutex_unlock(&this->mutex);
  
  return 0;
}


/* end FIFO --------------------------- */







typedef struct {
    int id;
    int size;
} eventHeader;


/* class to store an event */
class daqEvent {
  public:
    eventHeader h;
    void *data;
    int nRef;       // number of references to the object. If non-zero, the object should not be deleted.
    
    daqEvent();
    daqEvent(int size); // allocate also data
    ~daqEvent();
    void dereference();
    void reference();
      
    pthread_mutex_t mxRef;
};
void daqEvent::dereference() {
    pthread_mutex_lock(&mxRef);
    nRef--;
    if (nRef<1) {       // was <=1
        pthread_mutex_unlock(&mxRef);
        delete this;
    } else {
        pthread_mutex_unlock(&mxRef);
    }
    return;
}
void daqEvent::reference() {
    pthread_mutex_lock(&mxRef);
    nRef++;
    pthread_mutex_unlock(&mxRef);
    return;
}
daqEvent::daqEvent() {
    h.id=-1;
    h.size=-1;
    data=NULL;
    nRef=1;
    pthread_mutex_init(&mxRef,NULL);
}
daqEvent::daqEvent(int size) {  

    /* copy of standard constructor
     * to be re-implemented with constructor delgate, c++11 / need gcc 4.7
     */
    h.id=-1;
    h.size=-1;
    data=NULL;
    nRef=1;
    pthread_mutex_init(&mxRef,NULL);
    
    data=malloc(size);
    if (data==NULL) {throw "Failed to allocate memory";}
    bzero(data,size);
    h.size=size;
}
daqEvent::~daqEvent() {
  pthread_mutex_lock(&mxRef);
  if (nRef) {nRef--;}
  if (nRef) {
      printf("Warning, trying to delete referenced object\n");
  }
  pthread_mutex_unlock(&mxRef);
  if (data!=NULL) {
    free(data);
  }
  pthread_mutex_destroy(&mxRef);
}
int globalEventId=0;
daqEvent *generateRandomEvent(int min, int max) {
  int sz;
  void *d;
  daqEvent *e;
  e=new daqEvent();
  //return e;
  
  if (e==NULL) {return NULL;}
  sz=(int)((max-min)*1.0*rand()/RAND_MAX);  
  d=malloc(sz);
  if (d==NULL) {
    delete e;
    return NULL;
  }
  int i;
  for (i=0;i<sz;i++) {
      ((char *)d)[i]=(char)i;
  }
  e->h.id=globalEventId++;
  e->h.size=sz;
  e->data=d;
  return e;
}









daqModule_producer::daqModule_producer(zdaqCtrl_config c): daqModule(c) {
  f_out=NULL;
}
daqModule_producer::~daqModule_producer() {
}
int daqModule_producer::setFifoOut(daqModule_fifo *f) {
  if (f_out!=NULL) {
    //delete f_out;
  }
  f_out=f;
  return 0;
}


daqModule_consumer::daqModule_consumer(zdaqCtrl_config c): daqModule(c) {
  f_in=NULL;
  th_stop_immediate=0;
}
daqModule_consumer::~daqModule_consumer() {
}
int daqModule_consumer::setFifoIn(daqModule_fifo *f) {
  if (f_in!=NULL) {
    //delete f_in;
  }
  f_in=f;
  return 0;
}




daqModule_producer_rand::daqModule_producer_rand(zdaqCtrl_config c): daqModule_producer(c) {
}
daqModule_producer_rand::~daqModule_producer_rand() {
}


int daqModule_producer_rand::do_loop(int maxItems) {

  if (f_out==NULL) {return 1;}
  for (int i=0;(i<maxItems) || (maxItems==0);i++) {
    //if (stats_nItems_out>=1000) {return 0;}
      
    daqEvent *ev=NULL;
    ev=generateRandomEvent(100,1000);
    //printf("REC: event %d: sz=%d p=%p data=%p\n",ev->h.id,ev->h.size,ev,ev->data);
    if (ev==NULL) {return 1;}   

    //cout << "write ev #" << i << endl;
    int timeout;
    if (i==0) {
      timeout=10000;
    } else {
      timeout=0;
    }
    int nb;
    nb=ev->h.size+sizeof(ev->h);
    if (f_out->write(ev,timeout)) {
      delete ev;
      break;
    } else {
        //printf ("rand FIFO write = %p  size %d\n",ev,ev->h.size);
        
      stats_nItems_out++;
      stats_nBytes_out+=nb;
      //cout << ev->h.size << "bytes" << endl;
    }
  }
  return 0;
}



daqModule_consumer_recordToFile::daqModule_consumer_recordToFile(zdaqCtrl_config c): daqModule_consumer(c) {
  filename=NULL;
  fd=NULL;
  nEvents=0;
  nBytes=0;
}

daqModule_consumer_recordToFile::~daqModule_consumer_recordToFile(){
  cleanup();
  setFile(NULL);
}


int daqModule_consumer_recordToFile::setFile(const char* f){
  if (filename!=NULL) {free(filename);}
  if (f==NULL) {return 0;}
  filename=strdup(f);
  if (filename==NULL) {return 1;}
  return 0;
}

int daqModule_consumer_recordToFile::do_loop(int maxItems) {
  daqEvent *ev=NULL;
  if (f_in==NULL) {return 1;}
  
  int status=0;
  for (int i=0;(i<maxItems) || (maxItems==0);i++) {
        
    //cout << "read loop " << i << " / " << maxItems << endl;
    int timeout;
    if (i==0) {
      timeout=10000;
    } else {
      timeout=0;
    }

    //cout << "try read " << nEvents << endl;
    if (f_in->read((void**)&ev,timeout)) {
      return 0;
    }
    //cout << "read ok " << nEvents << endl;
    if (ev==NULL) {return 1;}
    for(;;) {
      if (fd!=NULL) {
        //printf("REC: event %d: sz=%d p=%p data=%p\n",ev->h.id,ev->h.size,ev,ev->data);
        if (fwrite(&ev->h,sizeof(ev->h),1,fd)!=1) {status=1; break;}
        /*for (int x=0;x<ev->h.size;x++) {
            printf("%d ",(int)(((char *)ev->data)[x]));
        }
        printf("\n");*/
        if (fwrite(ev->data,ev->h.size,1,fd)!=1) {status=1; break;}
      }
      delete ev;
      nEvents++;
      nBytes+=ev->h.size+sizeof(ev->h);
      
      stats_nItems_out++;
      stats_nBytes_out+=ev->h.size+sizeof(ev->h);
      break;
    }
    if (status) {
       cout << "write failed" << endl;
    }
    delete ev;
  }
  return status;
}


int daqModule_consumer_recordToFile::setup() {
  cout << "recordToFile setup() " << endl;
  nEvents=0;
  nBytes=0;
  if (filename==NULL) {return 0;}
  cout << "Opening " << filename << endl;
  fd=fopen(filename,"w");
  if (fd==NULL) {return -1;}
  return 0;
}
int daqModule_consumer_recordToFile::cleanup() {
  if (fd!=NULL) {
    fclose(fd);
    cout << "fd closed" << endl;
  }
  fd=NULL; 
  return 0;
}

int daqModule_consumer_recordToFile::exec_INIT() {
  return setup();
}
int daqModule_consumer_recordToFile::exec_RELEASE() {
  cout << getName() << " number of events " << nEvents << endl;
  cout << getName() << " number of bytes " << nBytes << endl;

  return cleanup();
}








#include <zmq.h>




/* a module to receive data remotely by ZMQ
 */

daqModule_producer_netrx::daqModule_producer_netrx(zdaqCtrl_config c): daqModule_producer(c) { 
    zmq_context = zmq_ctx_new ();
    zmq_responder = zmq_socket (zmq_context, ZMQ_PULL);
    zmq_path=NULL;
    
    pendingEvent=NULL;
}

daqModule_producer_netrx::~daqModule_producer_netrx(){
    cleanup();
    zmq_close (zmq_responder);
    zmq_ctx_destroy (zmq_context);
    if (zmq_path!=NULL) {free(zmq_path);}
    
    if (pendingEvent!=NULL) {
        delete((daqEvent*)pendingEvent);
    }
}

int daqModule_producer_netrx::do_loop(int maxItems) {
  daqEvent *ev=NULL;
  int status=0;
  
  char buffer [100000];
  int l;
  //printf("rx ...\n");
  
  if (f_out==NULL) return -1;
  if (f_out->isFull()) return 0;  // don't bother create event if fifo full...
  
  if (pendingEvent==NULL) {
    l=zmq_recv (zmq_responder, buffer, sizeof(buffer), ZMQ_DONTWAIT); //ZMQ_DONTWAIT
    if (l==-1) {
        if (errno==EAGAIN) {
            // nothing waiting for us
            //printf("rx nothing\n");
        } else {
            printf("rx error %s\n",zmq_strerror (errno));
        }
        //usleep(100000);
    } else {
        //printf("Received %d bytes\n",l);
  
        int more;
        size_t more_size = sizeof more;
        if (zmq_getsockopt (zmq_responder, ZMQ_RCVMORE, &more, &more_size)!=0) {return 1;}
        if (!more) {
            return 1; // we expect a 2-part message
        }
 
        // create event
        daqEvent *e;
        if (l!=sizeof(e->h)) {return 1;}
        e=new daqEvent();
        if (e==NULL) {return 1;}
        memcpy(&e->h,buffer,sizeof(e->h));
        e->data=malloc(e->h.size);
        // check result
        pendingEvent=e;
    }
  }

  if (pendingEvent!=NULL) {
    daqEvent *e;
    e=(daqEvent *)pendingEvent;
    if (e->data==NULL) {return 1;}
    l=zmq_recv (zmq_responder, e->data, e->h.size, ZMQ_DONTWAIT); //ZMQ_DONTWAIT
    if (l==-1) {
        if (errno==EAGAIN) {
            // nothing waiting for us
            //printf("rx nothing\n");
        } else {
            printf("rx error %s\n",zmq_strerror (errno));
        }
        //usleep(100000);
    } else {
        //printf("Received %d bytes\n",l);

        int more;
        size_t more_size = sizeof more;
        if (zmq_getsockopt (zmq_responder, ZMQ_RCVMORE, &more, &more_size)!=0) {return 1;}
        if (more) {
            delete e;
            return 1; // we expect a 2-part message
        }
        int nb;
        nb=e->h.size;
        
        if (f_out->write(e,-1)) {
          delete e;
          return 1;
        }  

        stats_nItems_in++;
        stats_nBytes_in+=nb;
        
        pendingEvent=NULL;        

    }
  }
  
  return status;
}

int daqModule_producer_netrx::setPath(const char *path) {
    if (zmq_path!=NULL) {
        free(zmq_path);
    }
    zmq_path=NULL;
    if (path!=NULL) {
        zmq_path=strdup(path);
    }
    return 0;
}
int daqModule_producer_netrx::setup() {
    if (zmq_path==NULL) {return -1;}
    int rc = zmq_bind (zmq_responder, zmq_path);  // e.g. "ipc:///tmp/5555"
    if (rc!=0) {return -1;}
    printf("rx ready\n");
    return 0;
}
int daqModule_producer_netrx::cleanup() {

    return 0;
}

int daqModule_producer_netrx::exec_INIT() {
  return setup();
}
int daqModule_producer_netrx::exec_RELEASE() {
  return cleanup();
}



/* a daq module to ship remotely */

daqModule_consumer_nettx::daqModule_consumer_nettx(zdaqCtrl_config c): daqModule_consumer(c) { 
    zmq_context = zmq_ctx_new ();
    zmq_requester = zmq_socket (zmq_context, ZMQ_PUSH);
    zmq_path=NULL;
}

daqModule_consumer_nettx::~daqModule_consumer_nettx(){
    cleanup();
    zmq_close (zmq_requester);
    zmq_ctx_destroy (zmq_context);
    if (zmq_path!=NULL) {free(zmq_path);}
}

int daqModule_consumer_nettx::do_loop(int maxItems) {
  daqEvent *ev=NULL;
  int status=0;
  
  //if (stats_nItems_out) {return 0;}
  
    if (f_in==NULL) {return 1;}
    if (f_in->read((void**)&ev,0)) {
      return 0;
    }
    if (ev==NULL) {return 1;}
    //printf ("nettx FIFO read = %p  size %d\n",ev,ev->h.size);
    if (ev->h.size<0) {return 1;}
  
  /* send in 2 parts: header + data */
  
  int l;
  l=zmq_send (zmq_requester, &ev->h, sizeof(ev->h), ZMQ_SNDMORE);
  if (l==-1) {
      printf("send #header error = %s\n",zmq_strerror (errno));
      status=-1;
  } else {
    l=zmq_send (zmq_requester, ev->data, ev->h.size, 0);
    if (l==-1) {
      printf("send #data error = %s\n",zmq_strerror (errno));
      status=-1;
    } else {
        stats_nItems_out++;
        stats_nBytes_out+= ev->h.size;
    }
  }
  
  ev->dereference();
  
  /*
   * l=zmq_send (zmq_requester, "Hello", 5, 0);
  if (l==-1) {
      printf("send error = %s\n",zmq_strerror (errno));
      status=-1;
  } else {
      //printf("Sent %d bytes\n",l);
      stats_nItems_out++;
      stats_nBytes_out+=l;
  }  
  //usleep(100000);
  */
  
  return status;
}


int daqModule_consumer_nettx::setPath(const char *path) {
    if (zmq_path!=NULL) {
        free(zmq_path);
    }
    zmq_path=NULL;
    if (path!=NULL) {
        zmq_path=strdup(path);
    }
    return 0;
}
int daqModule_consumer_nettx::setup() {
    if (zmq_path==NULL) {return -1;}   
    int rc = zmq_connect (zmq_requester, zmq_path);
    if (rc!=0) {
        printf("tx connect error : %s\n",zmq_strerror(errno));
        return -1;
    }
    printf("tx connected success\n");

    return 0;
}
int daqModule_consumer_nettx::cleanup() {

    return 0;
}

int daqModule_consumer_nettx::exec_INIT() {
  return setup();
}
int daqModule_consumer_nettx::exec_RELEASE() {
  return cleanup();
}


/******************************
daqModule_consumer_dummy
******************************/
 daqModule_consumer_dummy::daqModule_consumer_dummy(zdaqCtrl_config c): daqModule_consumer(c){
 
 }
 daqModule_consumer_dummy::~daqModule_consumer_dummy() {
     
 }
int daqModule_consumer_dummy::do_loop(int maxItems) {
  daqEvent *ev=NULL;
  if (f_in==NULL) {return 1;}
  
  int status=0;
  for (int i=0;(i<maxItems) || (maxItems==0);i++) {
        
    int timeout;
    if (i==0) {
      timeout=10000;
    } else {
      timeout=0;
    }
    if (f_in->read((void**)&ev,timeout)) {
      return 0;
    }
    if (ev==NULL) {return 1;}
    stats_nItems_in++;
    stats_nBytes_in+=ev->h.size+sizeof(ev->h);  
    
    //printf ("dummy FIFO read = %p  size %d\n",ev,ev->h.size);
    ev->dereference();    
  }
  return status;
}


/******************************
daqModule_producer_dummy
******************************/
 daqModule_producer_dummy::daqModule_producer_dummy(zdaqCtrl_config c): daqModule_producer(c){
      daqEvent* ev;
      ev=new daqEvent(100000000);
      printf("Created static event of size %d + header %lu = %lu bytes\n",ev->h.size,sizeof(ev->h),ev->h.size+sizeof(ev->h));
      this->myEvent=ev;      
  }
 daqModule_producer_dummy::~daqModule_producer_dummy() {
     ((daqEvent*)myEvent)->dereference();
 }
int daqModule_producer_dummy::do_loop(int maxItems) {
  daqEvent *ev=NULL;
  ev=(daqEvent *)myEvent;
  if (f_out==NULL) {return 1;}
  
  int status=0;
  for (int i=0;(i<maxItems) || (maxItems==0);i++) {
        
    int timeout;
    if (i==0) {
      timeout=10000;
    } else {
      timeout=0;
    }
    
    ev->reference();
    if (f_out->write(ev,timeout)) {
      break;
    }    
    stats_nItems_out++;
    stats_nBytes_out+=ev->h.size+sizeof(ev->h);    
  }
  return status;
}
