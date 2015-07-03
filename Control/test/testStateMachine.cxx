#include <StateMachine.h>
#include <unistd.h>

//using namespace StateMachine;

int main() {
  try {
    DirectoryConfig dCfg("localhost:2181");
    Directory d(dCfg);
    
    ControlObject obj("test",&d);
    obj.setState("READY");
    sleep(10);
    printf("done\n");

  }
  catch (const char *e) {
    printf("error : %s\n",e);
  }
  return 0;
}
