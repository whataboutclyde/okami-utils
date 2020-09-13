#ifndef OKAMI_UTILS_COMMON_H
#define OKAMI_UTILS_COMMON_H

#include <cstdint>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

// I don't want to use STL tuples because I want to ensure
// it's packed for reading into.
#pragma pack(push, 1)
struct UInt8Tuple {
  uint8_t x;
  uint8_t y;
  uint8_t z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Int8Tuple {
  int8_t x;
  int8_t y;
  int8_t z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Int16Tuple {
  int16_t x;
  int16_t y;
  int16_t z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FloatTuple {
  float x;
  float y;
  float z;
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