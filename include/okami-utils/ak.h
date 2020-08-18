#ifndef OKAMI_UTILS_AK_H
#define OKAMI_UTILS_AK_H

#include <fstream>
#include <cstdint>
using namespace std;

namespace OKAMI_UTILS {

#pragma pack(push, 1)
struct AKHeader {
  uint64_t first8are0;    //Always 00 00 00 00 00 00 00 00
  uint64_t first_offset;  //Always 88 00 00 00 00 00 00 00
  uint64_t second_offset;
  uint64_t third_offset;
  uint64_t fourth_offset;
  uint16_t unknown1;      // 01-33 but not 32
  uint16_t unknown2;      // 01-23 but not 15, 18, 1f
  uint16_t unknown3;      // 01-33 but not 2d because it's a 3d game duh.
  uint16_t unknown4;      // 03-1356, tons of variance.
  int32_t float0;
  int32_t float1;
  int32_t float2;
  int32_t float3;
  int32_t float4;
  int32_t float5;
  int32_t float6;
  int32_t float7;
  int32_t float8;
  int32_t float9;
  uint32_t alwayszero;
  uint32_t unknown5;      // Always 00-FF (not all values though)
  uint32_t padding1[4];
  uint64_t fifth_offset;
  uint32_t padding2[4];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct AKEntry {
  
};
#pragma pack(pop)

class AK {
  private:
    void cleanup();
    AKHeader header;
  public:
    AK() {};
    AK(char* path);
    ~AK();
    bool parse_file(ifstream& fin);
    bool parse_file(char* path);
    // void write_file(ofstream& fout);
    // bool write_file(char* path);
    int size();
}; // class AK

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AK_H