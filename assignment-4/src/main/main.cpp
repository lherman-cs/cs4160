#include <signal.h>
#include <ctime>
#include "core/engine.h"
#include "core/event.h"

int main(int, char* []) {
  srand(time(NULL));
  signal(SIGPIPE, SIG_IGN);

  try {
    Engine engine;
    engine.play();
  } catch (const string& msg) {
    std::cout << msg << std::endl;
  }
  // catch (...) {
  //   std::cout << "Oops, someone threw an exception!" << std::endl;
  // }

  return 0;
}
