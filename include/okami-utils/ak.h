#ifndef OKAMI_UTILS_AK_H
#define OKAMI_UTILS_AK_H

#include <fstream>
#include <filesystem>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <okami-utils/common.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

// #ifndef OKAMI_PS2_BUILD
// #define OKAMI_PS2_BUILD
// #endif

#ifdef OKAMI_PS2_BUILD
#define OFFSET_TYPE uint32_t
#else
#define OFFSET_TYPE uint64_t
#endif

#pragma pack(push, 1)
struct AKHeader {
  OFFSET_TYPE first8are0;    //Always 00 00 00 00 00 00 00 00
  OFFSET_TYPE purple_offset;  //Always 88 00 00 00 00 00 00 00 - color names are to match my spreadsheet because I don't know what they are yet.
  OFFSET_TYPE blue_offset;
  OFFSET_TYPE coordinates_offset;
  OFFSET_TYPE indices_offset;
  uint16_t unknown1;      // 01-33 but not 32
  uint16_t unknown2;      // 01-23 but not 15, 18, 1f
  uint16_t unknown3;      // 01-33 but not 2d because it's a 3d game duh.
  uint16_t coordinate_count; 
  float min_x;
  float max_x;
  float min_y;
  float max_y;
  float min_z;
  float max_z;
  float float6;
  float float7;
  float float8;
  float float9;
  uint32_t alwayszero;
  uint32_t unknown5;      // Always 00-FF (not all values though)
  uint32_t unknown6;      // 00-FF except broken/dev areas have FFFF sometimes. Most are 00 though.
  #ifdef OKAMI_PS2_BUILD
  uint32_t padding1;
  #else
  uint32_t padding1[3];   // Always 00.
  #endif
  OFFSET_TYPE vector_normals_offset;
  OFFSET_TYPE padding2[2];   // Always 00.
};
#pragma pack(pop)

// So the interesting part of this bit is in the files I've looked at, 
// unknown1 varies but unknown 2-5 is the same for every entry.
#pragma pack(push, 1)
struct AKIndicesEntry {
  int16_t coord_index[3];
  uint8_t unknown0; // At least partly which way you can pass through. Values 00-99
  uint8_t unknown1; // 00, 01, 02, 04, 06, 08, 09
  uint8_t unknown2; // 00, 01, 02, 20, 80, 84, 88, c0, c4, c8, d0
  uint8_t unknown3o0 : 1; // 00, 01, 02, 04, 08, 10, 14, 20, 21, 30, 40, 41, 50, 60, 80, 90
  uint8_t unknown3o1 : 1;
  uint8_t unknown3o2 : 1;
  uint8_t unknown3o3 : 1;
  uint8_t ignore_touched : 1;
  uint8_t damaging : 1;
  uint8_t liquid : 1;
  uint8_t void_out : 1;
  uint8_t unknown4; // Bunch of values.
  uint8_t unknown5; // 00-05, 07, 08, 10, 20, 40, 80, 81, 83
  uint8_t unknown6; // At least partly terrain? 02 water, 04 holy smoke grass, 10 lava... 00-0a, 0c, 0d, 0f-16, 18, 1d - 80 lava, but 36 of 0d, 39 of 1d
  uint8_t unknown7; // 00, 01, 02
  uint8_t unknown8; // 00, 40, c0
  uint8_t unknown9; // 00, 02, 04, 05, 08, 0a, 0c, 0e, 20, 40, 42, 4c, 60, 80, a0, e0
};
#pragma pack(pop)

class AK : public FileReader {
  private:
    void cleanup();
    // AKHeader header;
    vector<PackedTuple<int16_t>> coords;
    vector<PackedTuple<int8_t>> vector_normals;
    vector<uint16_t> indices;
  public:
    AK() {static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "Platform doesn't support this directly.");};
    ~AK();
    AKHeader header;
    bool process_file(ifstream& fin);
    bool process_file(ifstream& fin, uint32_t start_offset=0);
    int num_coordinates();
    PackedTuple<int16_t>* get_coordinates();
    FloatConstraints get_constraints();
    PackedTuple<int8_t>* get_vector_normals();
    int num_indices();
    int num_index_sets();
    uint16_t* get_index_sets();
    int get_gltf_position_size();
    int get_gltf_normal_size();
    int get_gltf_indices_size();
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
}; // class AK

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AK_H