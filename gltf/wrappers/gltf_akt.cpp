#include "gltf_akt.h"

namespace OKAMI_UTILS {

int GLTFAKT::get_gltf_buffer_size() {
  int size = 0;
  for (vector<AKTData>::iterator it=data.begin(); it!=data.end(); it++)
    size += it->ak.get_gltf_buffer_size();
  return size;
}

void GLTFAKT::dump_gltf_binary(ofstream& fout) {
  for (vector<AKTData>::iterator it=data.begin(); it!=data.end(); it++)
    it->ak.dump_gltf_binary(fout);
}

} // namespace OKAMI_UTILS