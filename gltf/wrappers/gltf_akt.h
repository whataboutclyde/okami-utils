#ifndef OKAMI_UTILS_GLTF_AKT_H
#define OKAMI_UTILS_GLTF_AKT_H

#include <okami-utils/akt.h>
#include <okami-utils/common.h>

namespace OKAMI_UTILS {

class GLTFAKT : public AKT {
  public:
    GLTFAKT() : AKT() {};
    ~GLTFAKT() {};
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
};

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_GLTF_SCA_H