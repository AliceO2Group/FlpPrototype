#include <StateMachine.h>
#include <unistd.h>

//using namespace StateMachine;



int main(int argc, char *argv[]) {

  const char *objName="test";
  
  if (argc>1) {
    objName=argv[1];
  }

  try {
    DirectoryConfig dCfg("localhost:2181");
    Directory d(dCfg);

    ControlClient obj(objName,&d);
    sleep(10);
    printf("done\n");

  }
  catch (const char *e) {
    printf("error : %s\n",e);
  }
  return 0;
}
