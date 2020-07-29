#ifndef _INCREMENTER_H_
#define _INCREMENTER_H_

class Incrementer 
{
  public:
    Incrementer(); 

    ~Incrementer();

    void increment();

    int getCount() const;

  private:
    int count_;
};

#endif
