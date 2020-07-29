#include "Incrementer.h"

class IncrementerWrapper 
{
  public:
    IncrementerWrapper(); 

    ~IncrementerWrapper();

    void increment();

    int getCount() const;

  private:
    Incrementer incrementer_;
};
