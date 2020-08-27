#ifndef OKAMI_UTILS_AK_H
#define OKAMI_UTILS_AK_H

#include <fstream>
#include <filesystem>
#include <cstdint>
#include <vector>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

#pragma pack(push, 1)
struct AKHeader {
  uint64_t first8are0;    //Always 00 00 00 00 00 00 00 00
  uint64_t purple_offset;  //Always 88 00 00 00 00 00 00 00 - color names are to match my spreadsheet because I don't know what they are yet.
  uint64_t blue_offset;
  uint64_t coordinates_offset;
  uint64_t indices_offset;
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
  uint32_t padding1[3];   // Always 00.
  uint64_t vector_normals_offset;
  uint32_t padding2[4];   // Always 00.
};
#pragma pack(pop)

// I should not be doing this but I'm doing it for now.
#pragma pack(push, 1)
struct AKVectorNormalEntry {
  int8_t x;
  int8_t y;
  int8_t z;
};
#pragma pack(pop)

// I should not be doing this but I'm doing it for now.
#pragma pack(push, 1)
struct AKCoordinateEntry {
  int16_t x;
  int16_t y;
  int16_t z;
};
#pragma pack(pop)

// So the interesting part of this bit is in the files I've looked at, 
// unknown1 varies but unknown 2-5 is the same for every entry.
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
    vector<AKVectorNormalEntry> vector_normals;
    vector<uint16_t> indices;
  public:
    AK() {};
    ~AK();
    bool parse_file(ifstream& fin, uint32_t start_offset);
    bool parse_file(fs::path path);
    bool parse_file(char* path);
    // TODO: Add purple+blue to parsing so I can write separate .AK files.
    // bool write_file(ofstream& fout);
    // bool write_file(fs::path path);
    // bool write_file(char* path);
    int num_coordinates();
    AKCoordinateEntry* get_coordinates();
    AKVectorNormalEntry* get_vector_normals();
    int num_index_sets();
    uint16_t* get_index_sets();
}; // class AK

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AK_H