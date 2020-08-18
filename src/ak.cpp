#include <iostream>
#include <iterator>
#include <okami-utils/ak.h>
using namespace std;

namespace OKAMI_UTILS {

AK::AK(char* path) {
  parse_file(path);
}

AK::~AK() {
  cleanup();
}

bool AK::parse_file(ifstream& fin) {
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

int AK::size() {
  return 0;
}

} // namespace OKAMI_UTILS
