#ifndef OKAMI_UTILS_SUBMESH_H
#define OKAMI_UTILS_SUBMESH_H

#include <fstream>
#include <okami-utils/common.h>
#include <okami-utils/mesh_division.h>

namespace OKAMI_UTILS {

#define MDB_ID_STRING 0x0062646D

#pragma pack(push, 1)
struct SubmeshHeader {
  uint32_t mdb_id;       // always 0062646D
  uint32_t submesh_type; // supposedly 20=static, 30=animated, but I've seen 40, 50, 60 (all hex)
  uint16_t mesh_id;
  uint16_t mesh_divisions;
  uint8_t padding[20];
};
#pragma pack(pop)

class Submesh {
  public:
    Submesh() {};
    ~Submesh() {};
    bool process_file(ifstream& fin);
    int size();
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
  //private:
    SubmeshHeader header;
    vector<MeshDivision> divisions;
};

} // namespace OKAMI_UTILS

#endif