#ifndef OKAMI_UTILS_AK_H
#define OKAMI_UTILS_AK_H

#include <fstream>
#include <cstdint>
#include <vector>
using namespace std;

namespace OKAMI_UTILS {

#pragma pack(push, 1)
struct AKHeader {
  uint64_t first8are0;    //Always 00 00 00 00 00 00 00 00
  uint64_t purple_offset;  //Always 88 00 00 00 00 00 00 00
  uint64_t blue_offset;
  uint64_t red_offset;
  uint64_t green_offset;
  uint16_t unknown1;      // 01-33 but not 32
  uint16_t unknown2;      // 01-23 but not 15, 18, 1f
  uint16_t unknown3;      // 01-33 but not 2d because it's a 3d game duh.
  uint16_t coordinate_count; 
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
  uint32_t unknown6;      // 00-FF except broken/dev areas have FFFF sometimes. Most are 00 though.
  uint32_t padding1[3];
  uint64_t yellow_offset;
  uint32_t padding2[4];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct AKYellowEntry {
  int8_t x;
  int8_t y;
  int8_t z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct AKCoordinateEntry {
  int16_t x;
  int16_t y;
  int16_t z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct AKIndicesEntry {
  int16_t coord_index[3];
  int16_t unknown1;
  int16_t unknown2;
  int16_t unknown3;
  int16_t unknown4;
  int16_t unknown5;
};
#pragma pack(pop)

class AK {
  private:
    void cleanup();
    AKHeader header;
    vector<AKCoordinateEntry> coords;
    vector<AKYellowEntry> yellow_stuff;
    vector<uint16_t> indices;
    uint32_t start_offset=0;
  public:
    AK() {};
    AK(char* path);
    AK(ifstream& fin, uint32_t offset);
    ~AK();
    bool parse_file(ifstream& fin);
    bool parse_file(char* path);
    // void write_file(ofstream& fout);
    // bool write_file(char* path);
    int num_coords();
    AKCoordinateEntry* get_coords();
    AKYellowEntry* get_yellow();
    int num_triangles();
    uint16_t* get_triangles();
}; // class AK

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AK_H