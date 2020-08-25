#include <iostream>
#include <iomanip>
#include <iterator>
#include <okami-utils/ak.h>
using namespace std;

namespace OKAMI_UTILS {

AK::AK(char* path) {
  parse_file(path);
}

AK::AK(ifstream& fin, uint32_t offset) {
  start_offset = offset;
  parse_file(fin);
}

AK::~AK() {
  cleanup();
}

bool AK::parse_file(ifstream& fin) {
  AKHeader header;
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

  int green_count = (header.yellow_offset - header.green_offset)/sizeof(AKIndicesEntry);
  fin.seekg(header.green_offset+start_offset, ios::beg);
  for (int i=0; i<green_count; i++) {
    AKIndicesEntry ientry;
    fin.read(reinterpret_cast<char*>(&ientry), sizeof(ientry));
    for (int n=0; n<3; n++)
      indices.push_back(ientry.coord_index[n]);
  }

  fin.seekg(header.red_offset+start_offset, ios::beg);
  for (int i=0; i<header.coordinate_count; i++) {
    AKCoordinateEntry centry;
    fin.read(reinterpret_cast<char*>(&centry), sizeof(centry));
    coords.push_back(centry);
  }

  return false;
}

bool AK::parse_file(char* path) {
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

// void AK::write_file(ofstream& fout) {
  // int file_size = sizeof(AK_ENTRY_COUNT_TYPE) + (AK_ENTRY_SIZE + AK_ENTRY_PADDING_SIZE)*entries.size();
  // int padding = 32-(file_size % 32); // padding for 32 byte alignment.
  // char* pad_buffer = (char*)calloc(sizeof(uint8_t),padding > AK_ENTRY_PADDING_SIZE ? padding : AK_ENTRY_PADDING_SIZE);

  // AK_ENTRY_COUNT_TYPE entry_count = entries.size();
  // fout.write(reinterpret_cast<char *>(&entry_count), sizeof(AK_ENTRY_COUNT_TYPE));
  // for (int i=0;i<entry_count;i++) {
  //   fout.write(reinterpret_cast<char *>(&entries[i]), AK_ENTRY_SIZE);
  //   //reading in padding for now
  //   //fout.write(pad_buffer, AK_ENTRY_PADDING_SIZE);
  // }
  // fout.write(pad_buffer, padding);
  // free(pad_buffer);
// }

// bool AK::write_file(char* path) {
  // ofstream fout(path, ios::out|ios::binary);
  // if (!fout.is_open()) {
  //   cerr << "Couldn't open file " << path << endl;
  //   return false;
  // }
  // write_file(fout);
  // fout.close();
  // return true;
//   return false;
// }

void AK::cleanup() {

}

int AK::num_coords() {
  return coords.size();
}

AKCoordinateEntry* AK::get_coords() {
  return coords.data();
}

int AK::num_triangles() {
  return indices.size()/3;
}

uint16_t* AK::get_triangles() {
  return indices.data();
}

} // namespace OKAMI_UTILS
