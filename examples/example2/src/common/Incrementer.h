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
