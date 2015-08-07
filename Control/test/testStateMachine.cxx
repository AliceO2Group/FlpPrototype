#include <StateMachine.h>
#include <unistd.h>

//using namespace StateMachine;


class myStateMachine:public ControlObject {
  public:
  int i=0;
  myStateMachine(const std::string objectName, Directory * dir):ControlObject(objectName,dir) {
    setState("READY");
  };
  ~myStateMachine() {
  };
  void executeCommand(const std::string command){
    i++;
    char buf[20];
    snprintf(buf,20,"STATE-%03d",i);
    setState(buf);
  };
};



int main(int argc, const char *argv[]) {
  
  const char *objName="test";
  if (argc>1) {
    objName=argv[1];
  }
    
    
  try {
    DirectoryConfig dCfg("localhost:2181");
    Directory d(dCfg);
    
    /*d.CreateNode("/oo"); 
    
    for (int i=0;i<100;i++) {
      char a[100];
      snprintf(a,100,"test%03d",i);
      ControlObject *obj;
      obj=new ControlObject(a,&d);
      obj->setState("READY");
    }
    */
    
    /*ControlObject obj(objName,&d);
    obj.setState("READY");
    */
    
    myStateMachine obj(objName,&d);
    
    sleep(100);
    printf("done\n");

  }
  catch (const char *e) {
    printf("error : %s\n",e);
  }
  catch (int e) {
    printf("error : %d\n",e);
  }
  
  return 0;
}
