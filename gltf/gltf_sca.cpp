#include "gltf_sca.h"

namespace OKAMI_UTILS {

const uint16_t GLTFSCA::indices[] = {
  0, 1, 2,
  0, 2, 3,
  1, 2, 6,
  1, 5, 6,
  2, 3, 6,
  3, 6, 7,
  0, 3, 4,
  3, 4, 7,
  0, 1, 4,
  1, 4, 5,
  4, 5, 6,
  4, 6, 7
};

int GLTFSCA::num_coordinates() {
  return 8;
}

FloatConstraints GLTFSCA::get_constraints(int i) {
  FloatConstraints constraints;
  SCAEntry e=entries[i];
  constraints.min_x = min(min(e.p1.x, e.p2.x), min(e.p3.x, e.p4.x));
  constraints.max_x = max(max(e.p1.x, e.p2.x), max(e.p3.x, e.p4.x));
  constraints.min_y = e.y;
  constraints.max_y = e.y+e.height;
  constraints.min_z = min(min(e.p1.z, e.p2.z), min(e.p3.z, e.p4.z));
  constraints.max_z = max(max(e.p1.x, e.p2.x), max(e.p3.x, e.p4.x));
  return constraints;
}

int GLTFSCA::num_indices() {
  return sizeof(indices)/sizeof(*indices);
}

int GLTFSCA::get_gltf_entry_position_size() {
  // Always 8 float points.
  return 8*3*sizeof(float);
}

int GLTFSCA::get_gltf_entry_indices_size() {
  // Always 8 float points.
  int pad = sizeof(indices)%4;
  if (pad != 0)
    pad = 4-pad;
  return sizeof(indices)+pad;
}

int GLTFSCA::get_gltf_buffer_size() {
  return get_gltf_entry_indices_size() + get_gltf_entry_position_size()*entries.size();
}

void GLTFSCA::dump_gltf_binary(ofstream& fout) {
  for (int i=0; i<num_indices(); i++)
    fout.write(reinterpret_cast<const char*>(&indices[i]), sizeof(*indices));
  int pad = sizeof(indices)%4;
  if (pad != 0) {
    pad = 4-pad;
    for (int i=0; i<pad; i++)
      fout.put('\0');
  }
  for (vector<SCAEntry>::iterator it=entries.begin(); it!= entries.end(); it++) {
    FloatTuple ft;
    ft.y = it->y;
    ft.x = it->p1.x;
    ft.z = it->p1.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.x = it->p2.x;
    ft.z = it->p2.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.x = it->p3.x;
    ft.z = it->p3.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.x = it->p4.x;
    ft.z = it->p4.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.y += it->height;
    ft.x = it->p1.x;
    ft.z = it->p1.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.x = it->p2.x;
    ft.z = it->p2.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.x = it->p3.x;
    ft.z = it->p3.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.x = it->p4.x;
    ft.z = it->p4.z;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  }
}

} // OKAMI_UTILS