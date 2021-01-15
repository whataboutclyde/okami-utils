#ifndef OKAMI_UTILS_COMMON_H
#define OKAMI_UTILS_COMMON_H

#include <cstdint>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

#pragma pack(push, 1)
template <typename T>
struct PackedTuple {
  T x;
  T y;
  T z;
};
#pragma pack(pop)

#pragma pack(push, 1)
template <typename T>
struct PackedPair {
  T x;
  T y;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FloatConstraints {
  float min_x;
  float max_x;
  float min_y;
  float max_y;
  float min_z;
  float max_z;
};
#pragma pack(pop)

class FileReader {
  public:
    FileReader() {};
    ~FileReader() {};
    virtual bool process_file(ifstream& fin) = 0;
    virtual bool parse_file(fs::path path);
    virtual bool parse_file(char* path);
    virtual void align(ifstream& fin, int bytes);
};

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_COMMON_H