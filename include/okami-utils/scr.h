#ifndef OKAMI_UTILS_SCR_H
#define OKAMI_UTILS_SCR_H

#include <cstdint>
using namespace std;

#define SCR_ID_STRING 0x00726373

#pragma pack(push, 1)
struct SCRHeader {
  uint32_t scr_id; // always 00726373
  uint32_t file_type; //0 for md, 1 for scr?
  uint32_t num_mdb;
  uint32_t padding; //possibly
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MDBHeader {
  uint32_t mdb_id; // always 0062646D
  uint32_t submesh_type; // supposedly 20=static, 30=animates
  uint16_t submesh_id; //maybe
  uint16_t mesh_divisions;
  uint8_t padding[20];
};
#pragma pack(pop);

#pragma pack(push, 1)
struct MDHeader {
  uint32_t v_offset; // offset from this spot to vertex info
  uint32_t unknown1; // 0?
  uint32_t itm_offset; //implicit texture mapping
  uint32_t tcw_offset; // texture colour weight
  uint32_t tuv_offset; // texture uv
  uint16_t vertices; // count
  uint16_t unknown2;
  //probably need padding
};
#pragma pack(pop);

#pragma pack(push, 1)
struct Vertex {
  int16_t x;
  int16_t y;
  int16_t z;
  int16_t conn;
};
#pragma pack(pop);

#endif //OKAMI_UTILS_SCR_H