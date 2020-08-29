#include <iostream>
#include <iomanip>
#include <iterator>
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

Int8Tuple* AK::get_vector_normals() {
  return vector_normals.data();
}


int AK::num_index_sets() {
  return indices.size()/3;
}

uint16_t* AK::get_index_sets() {
  return indices.data();
}

void AK::dump_binary(ofstream& fout) {
  int pad;
  for (vector<Int16Tuple>::iterator it=coords.begin(); it!=coords.end(); it++)
    fout.write(reinterpret_cast<char *>(&*it), sizeof(Int16Tuple));
  pad = coords.size()*sizeof(Int16Tuple)%2;
  for (int i=0; i<pad; i++)
    fout.put('\0');
  for (vector<Int8Tuple>::iterator it=vector_normals.begin(); it!=vector_normals.end(); it++)
    fout.write(reinterpret_cast<char *>(&*it), sizeof(Int8Tuple));
  pad = vector_normals.size()*sizeof(Int8Tuple)%2;
  for (int i=0; i<pad; i++)
    fout.put('\0');
  for (vector<uint16_t>::iterator it=indices.begin(); it!=indices.end(); it++)
    fout.write(reinterpret_cast<char *>(&*it), sizeof(uint16_t));
  pad = coords.size()*sizeof(Int16Tuple)%2;
  for (int i=0; i<pad; i++)
    fout.put('\0');
}

} // namespace OKAMI_UTILS
