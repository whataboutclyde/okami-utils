#define _USE_MATH_DEFINES
#include <math.h>
#include "gltf_zone.h"
using namespace std;

namespace OKAMI_UTILS {

const uint16_t GLTFZone::quad_indices[] = {
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

GLTFZone::GLTFZone() : Zone() {
  for (int i=0; i<ZONE_CYLINDER_SLICE_COUNT; i++) {
    int p1 = (i+1)*2;
    int p2 = (i+1)*2+1;
    int p3 = i < ZONE_CYLINDER_SLICE_COUNT-1 ? (i+2)*2 : 2;
    int p4 = i < ZONE_CYLINDER_SLICE_COUNT-1 ? (i+2)*2+1 : 3;

    // Bottom face
    cyl_indices[i*12+0]  = 0;
    cyl_indices[i*12+1]  = p1;
    cyl_indices[i*12+2]  = p3;

    // Side face 1
    cyl_indices[i*12+3]  = p1;
    cyl_indices[i*12+4]  = p3;
    cyl_indices[i*12+5]  = p4;

    // Side face 2
    cyl_indices[i*12+6]  = p1;
    cyl_indices[i*12+7]  = p2;
    cyl_indices[i*12+8]  = p4;

    // Top face
    cyl_indices[i*12+9]  = 1;
    cyl_indices[i*12+10] = p2;
    cyl_indices[i*12+11] = p4;
  }
}

int GLTFZone::num_quad_coordinates() {
  return 8;
}

int GLTFZone::num_cyl_coordinates() {
  return ZONE_CYLINDER_VERTICES_COUNT;
}

FloatConstraints GLTFZone::get_constraints(int i) {
  FloatConstraints constraints;
  ZoneEntry e=entries[i];
  constraints.min_x = min(min(e.p1.x, e.p2.x), min(e.p3.x, e.p4.x));
  constraints.max_x = max(max(e.p1.x, e.p2.x), max(e.p3.x, e.p4.x));
  constraints.min_y = e.y;
  constraints.max_y = e.y+e.height;
  constraints.min_z = min(min(e.p1.z, e.p2.z), min(e.p3.z, e.p4.z));
  constraints.max_z = max(max(e.p1.x, e.p2.x), max(e.p3.x, e.p4.x));
  return constraints;
}

int GLTFZone::num_quad_indices() {
  return sizeof(quad_indices)/sizeof(*quad_indices);
}

int GLTFZone::num_cyl_indices() {
  return ZONE_CYLINDER_SLICE_INDICES;
}

int GLTFZone::get_gltf_quad_position_size() {
  // Always 8 sets of 3 float points.
  return 8*3*sizeof(float);
}

int GLTFZone::get_gltf_cyl_position_size() {
  //                             #coords*3 floats
  return ZONE_CYLINDER_VERTICES_COUNT*3*sizeof(float);
}

int GLTFZone::get_gltf_quad_indices_size() {
  // Always 8 float points.
  int pad = sizeof(quad_indices)%4;
  if (pad != 0)
    pad = 4-pad;
  return sizeof(quad_indices)+pad;
}

int GLTFZone::get_gltf_cyl_indices_size() {
  int pad = sizeof(cyl_indices)%4;
  if (pad != 0)
    pad = 4-pad;
  return sizeof(cyl_indices)+pad;
}

int GLTFZone::get_gltf_buffer_size() {
  int size = get_gltf_quad_indices_size() + get_gltf_cyl_indices_size();
  for (vector<ZoneEntry>::iterator it=entries.begin(); it!=entries.end(); it++)
    size += it->zone_shape == OKAMI_UTILS::ZONE_SHAPE_QUAD_PRISM ? get_gltf_quad_position_size() : get_gltf_cyl_position_size();
  return size;
}

void GLTFZone::dump_gltf_binary(ofstream& fout) {
  fout.write(reinterpret_cast<const char*>(quad_indices), sizeof(quad_indices));
  int pad = sizeof(quad_indices)%4;
  if (pad != 0) {
    pad = 4-pad;
    for (int i=0; i<pad; i++)
      fout.put('\0');
  }

  fout.write(reinterpret_cast<const char*>(cyl_indices), sizeof(cyl_indices));
  pad = sizeof(cyl_indices)%4;
  if (pad != 0) {
    pad = 4-pad;
    for (int i=0; i<pad; i++)
      fout.put('\0');
  }

  for (vector<ZoneEntry>::iterator it=entries.begin(); it!= entries.end(); it++) {
    if (it->zone_shape == ZONE_SHAPE_QUAD_PRISM)
      dump_gltf_quad_coords(fout, *it);
    else if (it->zone_shape == ZONE_SHAPE_CYLINDER)
      dump_gltf_cyl_coords(fout, *it);
  }
}

void GLTFZone::dump_gltf_quad_coords(ofstream& fout, ZoneEntry entry) {
  FloatTuple ft;
  ft.y = entry.y;
  ft.x = entry.p1.x;
  ft.z = entry.p1.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.x = entry.p2.x;
  ft.z = entry.p2.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.x = entry.p3.x;
  ft.z = entry.p3.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.x = entry.p4.x;
  ft.z = entry.p4.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.y += entry.height;
  ft.x = entry.p1.x;
  ft.z = entry.p1.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.x = entry.p2.x;
  ft.z = entry.p2.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.x = entry.p3.x;
  ft.z = entry.p3.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.x = entry.p4.x;
  ft.z = entry.p4.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
}

void GLTFZone::dump_gltf_cyl_coords(ofstream& fout, ZoneEntry entry) {
  FloatTuple ft;
  ft.y = entry.y;
  ft.x = entry.p1.x;
  ft.z = entry.p1.z;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  ft.y = entry.y + entry.height;
  fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));

  float angle = 360.0f / ZONE_CYLINDER_SLICE_COUNT;

  for (int i=0; i<ZONE_CYLINDER_SLICE_COUNT; i++) {
    ft.y = entry.y;
    ft.x = entry.p1.x + entry.radius * cos(angle*i*M_PI/180.f);
    ft.z = entry.p1.z + entry.radius * sin(angle*i*M_PI/180.f);
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
    ft.y = entry.y+entry.height;
    fout.write(reinterpret_cast<char*>(&ft), sizeof(FloatTuple));
  }
}

} // OKAMI_UTILS