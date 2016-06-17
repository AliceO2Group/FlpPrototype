#include <Control/BaseControl.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {

  RuntimeControlledObject o("test2");
  RuntimeControlEngine e;
  e.registerObject(&o);

  sleep(60);
  return 0;
}
