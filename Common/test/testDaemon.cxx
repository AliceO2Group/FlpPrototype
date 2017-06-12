#include <Common/Daemon.h>

int main(int argc, char* argv[]) {
  Daemon d(argc,argv);
  return d.run();
}
