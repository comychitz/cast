
namespace Cast {

  class Cast {
    public:
      Cast(const std::string &cwd); 

      ~Cast();

      int build(const std::string &dir);   

      inline
      std::string topInclude() const {
        return top_ + "/build/include";
      }

      inline
      std::string topLib() const {
        return top_ + "/build/lib";
      }

      inline
      std::string topBin() const {
        return top_ + "/build/bin";
      }

    private:
      bool linkFiles(const Config &cfg, const std::string &dest);

    private:
      std::string top_;
  };
}
