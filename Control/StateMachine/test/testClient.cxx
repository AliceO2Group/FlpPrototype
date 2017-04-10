#include "ControlStateMachine/StateMachine.h"
#include <unistd.h>

//using namespace StateMachine;



int main(int argc, char *argv[]) {

  const char *objName="test";
  int autoExit=1;

  if (argc>1) {
    objName=argv[1];
  }

  try {
    DirectoryConfig dCfg("localhost:2181");
    Directory d(dCfg);

    ControlClient obj(objName,&d);
    if (autoExit) {
      sleep(3);
    } else {
      for (;;) {
        sleep(1);
      }
    }
    printf("done\n");

  }
  catch (const char *e) {
    printf("error : %s\n",e);
  }
  return 0;
}
