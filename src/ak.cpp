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

bool AK::parse_file(ifstream& fin, uint32_t start_offset = 0) {
  fin.read(reinterpret_cast<char*>(&header),sizeof(header));

  //cout << path << endl;
  if (header.first8are0 != 0)
    cout << "FUCK first 8: " << hex << setfill('0') << setw(8) << header.first8are0 << endl;
  if (header.purple_offset != 0x88)
    cout << "FUCK purple offset: " << hex << setfill('0') << setw(8) << header.purple_offset << endl;

  // cout << "unknown1: " << hex << setfill('0') << setw(4) << header.unknown1 << endl; // 01-33 but not 32
  // cout << "unknown2: " << hex << setfill('0') << setw(4) << header.unknown2 << endl; // 01-23 but not 15, 18, 1f
  // cout << "unknown3: " << hex << setfill('0') << setw(4) << header.unknown3 << endl; // 01-33 but not 2d
  // cout << "unknown4: " << hex << setfill('0') << setw(4) << header.unknown4 << endl; // 03-1356 but a lot missing

  // if (header.fourth_offset-header.second_offset != 256) {
  //   cout << "fml not always same length: " << header.fourth_offset-header.second_offset << endl;
  // }

  if (header.alwayszero != 0)
    cout << "FUCK alwayszero: " << hex << setfill('0') << setw(8) << header.alwayszero << endl;

  for (int i=0;i<3;i++) {
    if (header.padding1[i]!=0)
      cout << "FUCK padding1 index " << dec << i << ": " << hex << header.padding1[i] << endl;
  }

  for (int i=0;i<4;i++) {
    if (header.padding2[i]!=0)
      cout << "FUCK padding2 index " << dec << i << ": " << hex << header.padding2[i] << endl;
  }

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
    Int8Tuple vnentry;
    fin.read(reinterpret_cast<char*>(&vnentry), sizeof(vnentry));
    vector_normals.push_back(vnentry);
  }

  fin.seekg(header.coordinates_offset+start_offset, ios::beg);
  for (int i=0; i<header.coordinate_count; i++) {
    Int16Tuple centry;
    fin.read(reinterpret_cast<char*>(&centry), sizeof(centry));
    coords.push_back(centry);
  }

  return true;
}

bool AK::parse_file(fs::path path) {
  cleanup();

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  bool ret = parse_file(fin);
  fin.close();
  return ret;
}

bool AK::parse_file(char* path) {
  return parse_file(fs::path(path));
}

int AK::num_coordinates() {
  return coords.size();
}

Int16Tuple* AK::get_coordinates() {
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

Int8Tuple* AK::get_vector_normals() {
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
  for (vector<Int16Tuple>::iterator it=coords.begin(); it!=coords.end(); it++) {
    FloatTuple floats;
    floats.x = (float)it->x;
    floats.y = (float)it->y;
    floats.z = (float)it->z;
    fout.write(reinterpret_cast<char *>(&floats), sizeof(floats));
  }
  
  for (vector<Int8Tuple>::iterator it=vector_normals.begin(); it!=vector_normals.end(); it++) {
    FloatTuple floats;
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
  return coords.size()*sizeof(FloatTuple);
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
