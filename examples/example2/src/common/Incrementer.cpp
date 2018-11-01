#include "Incrementer.h"

Incrementer::Incrementer() : count_(0) {
}

Incrementer::~Incrementer() {
}

void Incrementer::increment() {
  count_++;
}

int Incrementer::getCount() const {
  return count_;
}
