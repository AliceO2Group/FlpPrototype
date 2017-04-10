/* 
 * File:   testMain.cpp
 * Author: chapelan
 *
 * Created on July 25, 2013, 11:56 AM
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "Control/zdaq.h"
#include "Control/zdaq_ctrl.h"

using namespace std;

/*
int main0() {
  daqModule *m;
  
  m=new daqModule();
  cout << "status= " << m->getStatusByName() << endl;
  m->executeCommand(INIT);
  m->executeCommand(START);
  sleep(3);
  m->executeCommand(STOP);
  m->executeCommand(RELEASE);
  delete m;
  
  daqModule_fifo *f;
  f=new daqModule_fifo(10);
  char *a1=(char *)"abc", *a2=(char *)"def", *a3=(char *)"ghi";
  f->write(a1,-1);
  f->write(a2,-1);
  void *r;
  while(!f->read(&r,0)) {
    cout << "got " << (char *)r << endl;
  }
  delete f;

if(0) {

  myTimer t;
  // t.test();
  
  // FIFO throughput
  int i;
  daqModule_fifo f1(10000000);
  i=0;
  t.reset();
  t.start();
  while(f1.write(a1,0)==0) {
    i++;
  }
  t.stop();
  cout << "FIFO write x " << i << " = " << t.getTime()/i << " seconds/write" << endl;

  void *d;
  i=0;
  t.reset();
  t.start();
  while(f1.read(&d,0)==0) {
    i++;
  }
  t.stop();
  cout << "FIFO read x " << i << " = " << t.getTime()/i << " seconds/read" << endl;
  
}  
  
  return 0;
}



int main1() {
  daqModule *ml[3];
  int nm=sizeof(ml)/sizeof(daqModule *);
  
  daqModule_fifo f(1000);
  daqModule_consumer_recordToFile rec;
  daqModule_producer_rand gen;
  
  gen.setFifoOut(&f);
//  rec.setFile("/tmp/data");
  rec.setFifoIn(&f);
  
  gen.setName("generator");
  rec.setName("recorder");
  f.setName("fifo");
  
  
  
  ml[0]=&f;
  ml[1]=&gen;
  ml[2]=&rec;  
  
  int t0,t1;
  
  
  int i;
  int err=0;
  for (i=0;i<nm;i++) {
    cout << "Module " << i << endl;
    ml[i]->executeCommand(INIT);
  }
  
  t0=(int)time(NULL);
  
  for (i=0;i<nm;i++) {
    cout << "Module " << i << endl;
    ml[i]->executeCommand(START);       
  }
  
  sleep(10);
  
  cout << "*** done ***" << endl;
  cout << "stop asynch" << endl;  

  for (i=0;i<nm;i++) {
    cout << "Module " << ml[i]->getName() << endl;
    ml[i]->executeCommandAsynch(STOP);       
  }
  

  t1=(int)time(NULL);
  
  // TODO: wait here pending exec completed
  cout << "waiting asynch" << endl;
  for (i=0;i<nm;i++) {
    cout << "Module " << ml[i]->getName() << endl;
    ml[i]->waitCommandAsynch(-1,NULL);       
  }
  cout << "waiting asynch done" << endl;

  for (i=0;i<nm;i++) {
    cout << "Module " <<  ml[i]->getName() << endl;
    ml[i]->executeCommand(RELEASE);       
  }

  int dt;
  dt=t1-t0;
  for (i=0;i<nm;i++) {
    cout << "Module " <<  ml[i]->getName() << " " << ml[i]->stats_nItems_out/dt << " items/sec  " << ml[i]->stats_nBytes_out/dt << " bytes/sec" <<endl;
  }
  
  
  return 0;
}



int main2() {
    //    zdaqCtrl_service s("/myservice2","127.0.0.1:2181");   //default zookeeper
    
    //sleep(3);
    //zdaqCtrl_client c ("/myservice2","127.0.0.1:2181");   //default zookeeper
    
    
    testStateObj *o;
    o=new testStateObj("/myobj","127.0.0.1:2181");
    sleep(3);
    zdaqCtrl_client c("/myobj","127.0.0.1:2181");
    sleep(2);
    int i;
    for (i=0;i<100;i++) {
      c.execCommand("cmd-test");
    }
    sleep(15);
    delete o;
    sleep(3);
    return 0;
}
*/

int main3(){
    const char *zcx="127.0.0.1:2181";
    
/*    daqModule m(cfg);
   
    zdaqCtrl_client c("/myobj","127.0.0.1:2181");
    sleep(2);
    c.execCommand("INIT");
    sleep(2);
    c.execCommand("START");
    sleep(2);
    c.execCommand("STOP");
    sleep(2);
    c.execCommand("RELEASE");
    sleep(20);
  */  
    
    /*    daqModule_fifo *f;
  f=new daqModule_fifo(zdaqCtrl_config("/fifo",zcx),10);
  char *a1=(char *)"abc", *a2=(char *)"def", *a3=(char *)"ghi";
  f->write(a1,-1);
  f->write(a2,-1);
  void *r;
  while(!f->read(&r,0)) {
    cout << "got " << (char *)r << endl;
  }
  delete f;
     */
 
  /*daqModule *ml[3];
  int nm=sizeof(ml)/sizeof(daqModule *);
  
  daqModule_fifo f(zdaqCtrl_config("/fifo",zcx),1000);
  daqModule_consumer_recordToFile rec(zdaqCtrl_config("/rec",zcx));
  daqModule_producer_rand gen(zdaqCtrl_config("/gen",zcx));
  
  gen.setFifoOut(&f);
//  rec.setFile("/tmp/data");
  rec.setFifoIn(&f);
 /* 
  gen.setName("generator");
  rec.setName("recorder");
  f.setName("fifo");
  */
  
  zdaqCtrl_client *cl[3];
  int nm=sizeof(cl)/sizeof(zdaqCtrl_client *);
  
  daqModule_fifo f(zdaqCtrl_config("/fifo",zcx),1000);
  daqModule_consumer_recordToFile rec(zdaqCtrl_config("/rec",zcx));
  daqModule_producer_rand gen(zdaqCtrl_config("/gen",zcx));
  rec.setFile("/dev/null");
  gen.setFifoOut(&f);
  rec.setFifoIn(&f);
  
  cl[0]=new zdaqCtrl_client ("/fifo",zcx);
  cl[1]=new zdaqCtrl_client ("/rec",zcx);
  cl[2]=new zdaqCtrl_client ("/gen",zcx);
    
  int t0,t1;
    
  int i;
  int err=0;
  for (i=0;i<nm;i++) {
    cout << "Module " << i << endl;
    cl[i]->execCommand("INIT");
  }

  for (i=0;i<nm;i++) {
    cout << "Module " << i << endl;
    cl[i]->execCommand("START");
  }
  sleep(10);
  
  for (i=0;i<nm;i++) {
    cout << "Module " << i << endl;
    cl[i]->execCommand("STOP");
  }
      
  for (i=0;i<nm;i++) {
    cout << "Module " << i << endl;
    cl[i]->execCommand("RELEASE");
  }
  return 0;
}


int main4() {
    const char *zcx="127.0.0.1:2181";
    
    daqModule_producer_rand gen(zdaqCtrl_config("/gen",zcx));
    zdaqCtrl_client c("/gen",zcx);
    
    gen.publishString("data1","val1");
    char buf[128];
    int bufsz=sizeof(buf);
    c.getData("data1",buf,&bufsz);
    printf("data=%s\n",buf);
    sleep(30);
    
    
    
    
    return 0;
}

int test5() {
    const char *zcx="127.0.0.1:2181";
    
    daqModule_consumer_nettx tx(zdaqCtrl_config("/nettx",zcx));
    daqModule_producer_netrx rx(zdaqCtrl_config("/netrx",zcx));
    
    zdaqCtrl_client ctx("/nettx",zcx);
    zdaqCtrl_client crx("/netrx",zcx);
    
    crx.execCommand("INIT");
    sleep(1);
    ctx.execCommand("INIT");
    sleep(1);
    crx.execCommand("START");
    sleep(1);
    ctx.execCommand("START");
    sleep(3);
    ctx.execCommand("STOP");
    crx.execCommand("STOP");
    sleep(3);
    
    crx.execCommand("RELEASE");
    ctx.execCommand("RELEASE");
    
    

    return 0;
}


/* used for tests Aug 2013 */
int test6() {
    const char *zcx="127.0.0.1:2181";
  
    zdaqCtrl_group g(zcx);
    
    g.addObject("/rand");
    g.addObject("/fifo");
    g.addObject("/dummy");
    

    myTimer t;
    
    int fs;
    for (fs=1;fs<=1000000;fs*=10) {
    
    daqModule_consumer_dummy crx(zdaqCtrl_config("/dummy",zcx));
    daqModule_producer_dummy ctx(zdaqCtrl_config("/rand",zcx));
    daqModule_fifo f(zdaqCtrl_config("/fifo",zcx),fs);
         
    ctx.setFifoOut(&f);
    crx.setFifoIn(&f);
    
    printf("FIFO size=%d\n",fs);
    

    g.execCommand("INIT");
    sleep(1);    
    g.execCommand("START");
    t.reset();
    t.start();
    sleep(5);       
    t.stop();
    g.execCommand("STOP");
    sleep(1);
    
    printf("%d items in %fs => %f items/s\n",crx.stats_nItems_in,t.getTime(),crx.stats_nItems_in/t.getTime());
    
    
    
    g.execCommand("RELEASE");    
    
    sleep(3);
    
    }
    

    
 /*   sleep(3);
    delete(g);
    g=new zdaqCtrl_group(zcx);
    sleep(3);
    delete(g);
    /*
    daqModule_consumer_dummy crx(zdaqCtrl_config("/dummy",zcx));
    daqModule_producer_rand ctx(zdaqCtrl_config("/rand",zcx));
    daqModule_fifo f(zdaqCtrl_config("/fifo",zcx),10);
    /*
    ctx.setFifoOut(&f);
    crx.setFifoIn(&f);
    f.
    f.execCommand("INIT");
    crx.execCommand("INIT");
    ctx.execCommand("INIT");

    crx.execCommand("START");
    f.execCommand("START");
    ctx.execCommand("START");

    sleep(3);
    
    ctx.execCommand("STOP");
    f.execCommand("STOP");
    crx.execCommand("STOP");
    
    crx.execCommand("RELEASE");
    ctx.execCommand("RELEASE");
    f.execCommand("RELEASE");
    
*/
    return 0;
}



int testFIFO() {
    const char *zcx="127.0.0.1:2181";
  
    zdaqCtrl_group g(zcx);
    
    g.addObject("/rand");
    g.addObject("/fifo");
    g.addObject("/dummy");   

    myTimer t;
    FILE *fp;
    fp=fopen("res.txt","w");
    if (fp==NULL) {return -1;}
    
    int fs;     // FIFO size
    int testSz[]={1,2,3,5,10,50,100,500,1000,5000,10000,50000,100000,500000,1000000,10000000};
   //  int testSz[]={500000,1000000,10000000};
    
    int ix;
    for (ix=0;ix<sizeof(testSz)/sizeof(int);ix++) {
        
        int n;
        
        double nmin,nmax,navg;

        for (n=0;n<5;n++) {
            
            fs=testSz[ix];

            daqModule_consumer_dummy crx(zdaqCtrl_config("/dummy",zcx));
            daqModule_producer_dummy ctx(zdaqCtrl_config("/rand",zcx));
            daqModule_fifo f(zdaqCtrl_config("/fifo",zcx),fs);

            ctx.setFifoOut(&f);
            crx.setFifoIn(&f);
  
            printf("FIFO size=%d\n",fs);

            g.execCommand("INIT");
            sleep(1);    
            g.execCommand("START");
            t.reset();
            t.start();
            sleep(5);       
            t.stop();
            g.execCommand("STOP");
            sleep(1);
            printf("%d items in %fs => %f items/s\n",crx.stats_nItems_in,t.getTime(),crx.stats_nItems_in/t.getTime());
            
            double v;
            v=crx.stats_nItems_in*1.0/t.getTime();
            if (n==0) {
                nmin=v;
                nmax=v;
                navg=v;
            } else {
                if (v<nmin) nmin=v;
                if (v>nmax) nmax=v;
                navg+=v;
            }
                       
        }
        navg=navg/n;
        printf("%d\t%f\t%f\t%f\n",fs,nmin,navg,nmax);
        fprintf(fp,"%d\t%f\t%f\t%f\n",fs,nmin,navg,nmax);
        fflush(fp);
    }
    fclose(fp);
    return 0;
}



int testIPC() {
    const char *zcx="127.0.0.1:2181";
    
    int fs=100;     // FIFO size
    
    myTimer t;

        daqModule_consumer_dummy dummy(zdaqCtrl_config("/dummy",zcx));
        daqModule_producer_dummy rand(zdaqCtrl_config("/rand",zcx));
        daqModule_fifo fifo1(zdaqCtrl_config("/fifo1",zcx),fs);
        daqModule_fifo fifo2(zdaqCtrl_config("/fifo2",zcx),fs);    
        
    daqModule_consumer_nettx tx(zdaqCtrl_config("/nettx",zcx));
    daqModule_producer_netrx rx(zdaqCtrl_config("/netrx",zcx));
    
    zdaqCtrl_client ctx("/nettx",zcx);
    zdaqCtrl_client crx("/netrx",zcx);
    zdaqCtrl_group g(zcx);    
    g.addObject("/rand");
    g.addObject("/fifo1");
    g.addObject("/fifo2");
    g.addObject("/dummy");
    g.execCommand("INIT");
    sleep(2);      
    
    rand.setFifoOut(&fifo1);
    tx.setFifoIn(&fifo1);
    
    rx.setFifoOut(&fifo2);
    dummy.setFifoIn(&fifo2);
    
    rx.setPath("ipc:///tmp/5555");      //inproc://#1
    tx.setPath("ipc:///tmp/5555");
    
    crx.execCommand("INIT");
    sleep(1);
    ctx.execCommand("INIT");
    sleep(1);
    
    g.execCommand("INIT");
    
    crx.execCommand("START");
    sleep(1);
    ctx.execCommand("START");
            t.reset();
            t.start();
            sleep(5);       
            t.stop();
            
    g.execCommand("START");            
    sleep(5);
    g.execCommand("STOP");
    
    ctx.execCommand("STOP");
    crx.execCommand("STOP");
    sleep(3);

printf("%d items in %fs => %f items/s\n",rx.stats_nItems_in,t.getTime(),rx.stats_nItems_in/t.getTime());
                   
   

    return 0;
}







int test8() {
    const char *zcx="127.0.0.1:2181";
    
    int fs=10;     // FIFO size

    myTimer t;

    daqModule_consumer_dummy dummy(zdaqCtrl_config("/dummy",zcx));
    daqModule_producer_dummy rand(zdaqCtrl_config("/rand",zcx));
    daqModule_fifo fifo1(zdaqCtrl_config("/fifo1",zcx),fs);
    daqModule_fifo fifo2(zdaqCtrl_config("/fifo2",zcx),fs);
    daqModule_consumer_nettx tx(zdaqCtrl_config("/nettx",zcx));
    daqModule_producer_netrx rx(zdaqCtrl_config("/netrx",zcx));
    
    zdaqCtrl_group g(zcx);    
    g.addObject("/rand");
    g.addObject("/fifo1");
    g.addObject("/fifo2");
    g.addObject("/dummy");
    g.addObject("/netrx");
    g.addObject("/nettx");
    
    
    rand.setFifoOut(&fifo1);
    tx.setFifoIn(&fifo1);    
    rx.setFifoOut(&fifo2);
    dummy.setFifoIn(&fifo2);
    
    rx.setPath("ipc:///tmp/5555");      //inproc://#1
    tx.setPath("ipc:///tmp/5555");
    
    
  /*rand.setFifoOut(&fifo1);
  tx.setFifoIn(&fifo1);
*/
    printf("FIFO size=%d\n",fs);

    g.execCommand("INIT");
    sleep(1);    
    g.execCommand("START");
    t.reset();
    t.start();
    sleep(15);       
    t.stop();
    g.execCommand("STOP");
    sleep(1);
    printf("%d items in %fs => %f items/s\n",dummy.stats_nItems_in,t.getTime(),dummy.stats_nItems_in/t.getTime());

    printf("%lld bytes in %fs => %f MB/s\n",dummy.stats_nBytes_in,t.getTime(),(dummy.stats_nBytes_in/(1024*1024))/t.getTime());
    
    return 0;
}








int main() {
  return testIPC();
}




/**************************/
/* TODO ideas

thread loop:  can be several threads in parallel
as result of init command
e.g. one thread per RORC in readout
thread_loop take as arg a LOOP_ID (0 if unique thread, or an ID>=1 when multi threads)


configuration of modules

TODO: define FIFOin and FIFOout as subclass of DAQFIFO to restrict operations possible on both sides

TODO: consumers: two policies for 'stop'. stop is normal -> empty FIFOs. otherwise 'abort', don't process FIFO.


FIFO: global timeout? set time once for all before all iterations?

need ASYNC execution of commands to avoid serialization


todo: need log system for time precision. pid/threadid
*/


/*
tests results:

rand+fifo(100)+rec() = 40GB/sec 895KHz
rand+fifo(1)+rec() = 610Hz
rand+fifo(1)+rec() = 744KHz
*/
