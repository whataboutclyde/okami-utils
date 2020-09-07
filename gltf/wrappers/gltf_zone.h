#ifndef OKAMI_UTILS_GLTF_ZONE_H
#define OKAMI_UTILS_GLTF_ZONE_H

#include <okami-utils/zone.h>
#include <okami-utils/common.h>

namespace OKAMI_UTILS {

#define ZONE_CYLINDER_SLICE_COUNT     20
#define ZONE_CYLINDER_VERTICES_COUNT  (2+2*ZONE_CYLINDER_SLICE_COUNT)   // 2 for center + 2 per slice
#define ZONE_CYLINDER_SLICE_TRIANGLES (ZONE_CYLINDER_SLICE_COUNT*4)     // 4 triangles per slice
#define ZONE_CYLINDER_SLICE_INDICES   (ZONE_CYLINDER_SLICE_TRIANGLES*3) // 3 indices per triangle

class GLTFZone : public Zone {
  public:
    GLTFZone();
    ~GLTFZone() {};
    int num_quad_coordinates();
    int num_cyl_coordinates();
    FloatConstraints get_constraints(int i);
    int num_quad_indices();
    int num_cyl_indices();
    int get_gltf_quad_position_size();
    int get_gltf_cyl_position_size();
    int get_gltf_quad_indices_size();
    int get_gltf_cyl_indices_size();
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
    void dump_gltf_quad_coords(ofstream& fout, ZoneEntry entry);
    void dump_gltf_cyl_coords(ofstream& fout, ZoneEntry entry);
  private:
    static const uint16_t quad_indices[];
    uint16_t cyl_indices[ZONE_CYLINDER_SLICE_INDICES];

};

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_GLTF_SCA_H