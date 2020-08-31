#ifndef OKAMI_UTILS_GLTF_SCA_H
#define OKAMI_UTILS_GLTF_SCA_H

#include <okami-utils/sca.h>
#include <okami-utils/common.h>

namespace OKAMI_UTILS {

class GLTFSCA : public SCA {
  public:
    GLTFSCA() : SCA() {};
    ~GLTFSCA() {};
    int num_coordinates();
    FloatConstraints get_constraints(int i);
    int num_indices();
    int get_gltf_entry_position_size();
    int get_gltf_entry_indices_size();
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
  private:
    static const uint16_t indices[];
};

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_GLTF_SCA_H