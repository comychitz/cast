#include "IncrementerWrapper.h"

IncrementerWrapper::IncrementerWrapper() {
}

IncrementerWrapper::~IncrementerWrapper() {
}

void IncrementerWrapper::increment() {
  incrementer_.increment();
}

int IncrementerWrapper::getCount() const {
  return incrementer_.getCount();
}
