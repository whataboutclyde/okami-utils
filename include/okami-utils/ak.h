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
    // AKHeader header;
    vector<Int16Tuple> coords;
    vector<Int8Tuple> vector_normals;
    vector<uint16_t> indices;
  public:
    AK() {static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "Platform doesn't support this directly.");};
    ~AK();
    AKHeader header;
    bool parse_file(ifstream& fin, uint32_t start_offset);
    bool parse_file(fs::path path);
    bool parse_file(char* path);
    // TODO: Add purple+blue to parsing so I can write separate .AK files.
    // bool write_file(ofstream& fout);
    // bool write_file(fs::path path);
    // bool write_file(char* path);
    int num_coordinates();
    Int16Tuple* get_coordinates();
    FloatConstraints get_constraints();
    Int8Tuple* get_vector_normals();
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