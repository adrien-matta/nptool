#ifndef RAYIN_HEADER
#define RAYIN_HEADER
#include <string>

class Rayin {
 public:
  Rayin(const std::string& filename, bool check=true);
  ~Rayin();
 private:
  bool isOwner;
};

#endif
