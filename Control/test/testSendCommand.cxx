#include <StateMachine.h>
#include <unistd.h>
#include <cstdlib>

//using namespace StateMachine;



int main(int argc, char *argv[]) {

  const char *objName="test";
  const char *cmd="MOVE";
  int nIter=10;
  
  if (argc>1) {
    objName=argv[1];
  }
  if (argc>2) {
    cmd=argv[2];
  }
  if (argc>3) {
    nIter=atoi(argv[3]);
  }

  try {
    DirectoryConfig dCfg("localhost:2181");
    Directory d(dCfg);

/*  ControlClient *obj[100];
    for (int i=0;i<100;i++) {
      char a[100];
      snprintf(a,100,"test%03d",i);
      obj[i]=new ControlClient(a,&d);
    }
    for (int i=0;i<1;i++) {
      char cmd[100];
      snprintf(cmd,100,"MOVE%0d",i);
      obj[i%100]->sendCommand(cmd);
    }
*/

    ControlClient obj(objName,&d);
    obj.sendCommand(cmd);
    for (int i=0;i<nIter;i++) {
      obj.sendCommand(cmd);
      /*
      char cmdl[100];
      snprintf(cmdl,100,"%s%0d",cmd,i);
      obj.sendCommand(cmdl);
      */
    }
    sleep(10);
    printf("done\n");

  }
  catch (const char *e) {
    printf("error : %s\n",e);
  }
  return 0;
}
