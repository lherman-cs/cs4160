#include <vector>

void poo(std::vector<int>& v) {
  if (v.size() == 0) return;

  auto it = v.begin();
  auto lastnum = *it;
  it++;

  // erase iterator
  while (it != v.end()) {
    if (lastnum % 3 == 0 && *it % 2 == 0)
      it = v.erase(it);
    else {
      lastnum = *it;
      it++;
    }
  }
}

int main() {}