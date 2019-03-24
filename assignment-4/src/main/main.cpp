// Brian Malloy        Data-Driven Object oriented Game Construction
#include "core/engine.h"
#include "player/human.h"

void callback(const SetBetEvent& e) {
  std::cout << "CALLBACK " << e.quantity << std::endl;
}

int main(int, char*[]) {
  Human h("Lukas");
  h.subscribe<SetBetEvent>(callback);

  try {
    Engine engine;
    engine.play();
  } catch (const string& msg) {
    std::cout << msg << std::endl;
  } catch (...) {
    std::cout << "Oops, someone threw an exception!" << std::endl;
  }
  return 0;
}
