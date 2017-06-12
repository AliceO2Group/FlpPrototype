// most simple daemon example
// uses dummy default doLoop() function from the class
// helps to test e.g. command line parameters settings

#include <Common/Daemon.h>

int main(int argc, char* argv[]) {
  Daemon d(argc,argv);
  return d.run();
}
