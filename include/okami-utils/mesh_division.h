#ifndef OKAMI_UTILS_MESH_DIVISION_H
#define OKAMI_UTILS_MESH_DIVISION_H

#include <cstdint>
#include <vector>
#include <okami-utils/common.h>
using namespace std;

namespace OKAMI_UTILS {

#define NOT_TRI_STRIP_CONN (uint16_t)0x8000

#pragma pack(push, 1)
struct MDHeader {
  uint32_t v_offset; // offset from this spot to vertex info
  uint32_t unknown1; // 0?
  uint32_t itm_offset; //implicit texture mapping
  uint32_t tcw_offset; // texture colour weight
  uint32_t tuv_offset; // texture uv
  uint16_t vertices; // count
  uint16_t texture_index; // File index of DDS in DDP
  //probably need padding
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCRVertex {
  PackedTuple<int16_t> coordinates;
  uint16_t conn;
};
#pragma pack(pop)

class MeshDivision {
  public:
    MeshDivision() {};
    ~MeshDivision() {};
    bool process_file(ifstream& fin);
    int size();
    int get_gltf_buffer_size();
    int num_indices();
    int get_gltf_indices_size();
    int num_coordinates();
    int get_gltf_vertices_size();
    int get_gltf_itm_size();
    int get_gltf_tcw_size();
    int get_gltf_tuv_size();
    void dump_gltf_binary(ofstream& fout);
  //private:
    MDHeader header;
    vector<SCRVertex> vertices;
    vector<int32_t> unknown1;
    vector<PackedPair<uint16_t>> itm;
    vector<uint32_t> tcw;
    vector<PackedPair<uint16_t>> tuv;
    vector<uint16_t> indices;

    void build_indices();
};

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_MESH_DIVISION_H