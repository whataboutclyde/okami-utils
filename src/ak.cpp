#include <iostream>
#include <iomanip>
#include <iterator>
#include <glm/gtc/packing.hpp>
#include <okami-utils/ak.h>
using namespace std;

namespace OKAMI_UTILS {

AK::~AK() {
  cleanup();
}

void AK::cleanup() {
  coords.clear();
  vector_normals.clear();
  indices.clear();
}

bool AK::process_file(ifstream& fin) {
  return process_file(fin, 0);
}

bool AK::process_file(ifstream& fin, uint32_t start_offset) {
  fin.read(reinterpret_cast<char*>(&header),sizeof(header));

  int green_count = (header.vector_normals_offset - header.indices_offset)/sizeof(AKIndicesEntry);
  fin.seekg(header.indices_offset+start_offset, ios::beg);
  for (int i=0; i<green_count; i++) {
    AKIndicesEntry ientry;
    fin.read(reinterpret_cast<char*>(&ientry), sizeof(ientry));
    for (int n=0; n<3; n++)
      indices.push_back(ientry.coord_index[n]);
  }

  fin.seekg(header.vector_normals_offset+start_offset, ios::beg);
  for (int i=0; i<header.coordinate_count; i++) {
    PackedTuple<int8_t> vnentry;
    fin.read(reinterpret_cast<char*>(&vnentry), sizeof(vnentry));
    vector_normals.push_back(vnentry);
  }

  fin.seekg(header.coordinates_offset+start_offset, ios::beg);
  for (int i=0; i<header.coordinate_count; i++) {
    PackedTuple<int16_t> centry;
    fin.read(reinterpret_cast<char*>(&centry), sizeof(centry));
    coords.push_back(centry);
  }

  return true;
}

int AK::num_coordinates() {
  return coords.size();
}

PackedTuple<int16_t>* AK::get_coordinates() {
  return coords.data();
}

FloatConstraints AK::get_constraints() {
  FloatConstraints constraints;
  constraints.min_x = header.min_x;
  constraints.max_x = header.max_x;
  constraints.min_y = header.min_y;
  constraints.max_y = header.max_y;
  constraints.min_z = header.min_z;
  constraints.max_z = header.max_z;
  return constraints;
}

PackedTuple<int8_t>* AK::get_vector_normals() {
  return vector_normals.data();
}

int AK::num_indices() {
  return indices.size();
}

int AK::num_index_sets() {
  return indices.size()/3;
}

uint16_t* AK::get_index_sets() {
  return indices.data();
}

void AK::dump_gltf_binary(ofstream& fout) {
  for (vector<PackedTuple<int16_t>>::iterator it=coords.begin(); it!=coords.end(); it++) {
    PackedTuple<float> floats;
    floats.x = (float)it->x;
    floats.y = (float)it->y;
    floats.z = (float)it->z;
    fout.write(reinterpret_cast<char *>(&floats), sizeof(floats));
  }
  
  for (vector<PackedTuple<int8_t>>::iterator it=vector_normals.begin(); it!=vector_normals.end(); it++) {
    PackedTuple<float> floats;
    glm::vec3 stuff{glm::unpackSnorm1x8(it->x), glm::unpackSnorm1x8(it->y), glm::unpackSnorm1x8(it->z)};
    stuff=glm::normalize(stuff);
    floats.x = stuff.x;
    floats.y = stuff.y;
    floats.z = stuff.z;
    fout.write(reinterpret_cast<char *>(&floats), sizeof(floats));
  }
  
  for (vector<uint16_t>::iterator it=indices.begin(); it!=indices.end(); it++)
    fout.write(reinterpret_cast<char *>(&*it), sizeof(uint16_t));

  int pad = (indices.size() * sizeof(uint16_t)) % 4;
  if (pad != 0) {
    for (int i=0; i<4-pad; i++)
      fout.put('\0');
  }

}

int AK::get_gltf_position_size() {
  return coords.size()*sizeof(PackedTuple<float>);
}

int AK::get_gltf_normal_size() {
  return vector_normals.size()*sizeof(glm::vec3);
}

int AK::get_gltf_indices_size() {
  int pad = (indices.size()*sizeof(uint16_t))%4;
  if (pad != 0)
    pad = 4-pad;
  return indices.size()*sizeof(uint16_t) + pad;
}

int AK::get_gltf_buffer_size() {
  return get_gltf_position_size()+get_gltf_normal_size()+get_gltf_indices_size();
}

} // namespace OKAMI_UTILS
