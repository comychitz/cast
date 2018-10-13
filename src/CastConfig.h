
namespace Cast {

  class Config {
    public:
      Config();

      ~Config() = default;

      void read(const std::string &cfg);

      const std::string &target() const;

      const std::vector<std::string> &subdirs() const;

      const std::string &cflags() const;

      const std::string &ldflags() const;

    private:
      std::map<std::string, std::string> cfg_;

  };

}
