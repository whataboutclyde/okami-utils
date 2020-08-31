#include <iostream>
#include <okami-utils/sca.h>
using namespace std;

namespace OKAMI_UTILS {

SCA::SCA(char* path) {
  parse_file(path);
}

SCA::~SCA() {
  cleanup();
}

bool SCA::parse_file(ifstream& fin) {
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char*>(&header),sizeof(header));

  if (header.header != MEH_HEADER_ID && header.header != SCA_HEADER_ID && header.header != SCI_HEADER_ID) {
    cerr << "Not a valid SCA file. Header: " << hex << header.header << endl;
    return false;
  }

  for (int i=0; i<header.entry_count;i++) {
    SCAEntry entry;
    fin.read(reinterpret_cast<char*>(&entry), SCA_ENTRY_SIZE);
    entries.push_back(entry);
  }

  return true;
}

bool SCA::parse_file(fs::path path) {
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

bool SCA::parse_file(char* path) {
  return parse_file(fs::path(path));
}

void SCA::write_file(ofstream& fout) {
  // int file_size = sizeof(ENTRY_COUNT_TYPE) + (SCAENTRY_SIZE + SCAENTRY_PADDING_SIZE)*entries.size();
  // int padding = 32-(file_size % 32); // padding for 32 byte alignment.
  // char* pad_buffer = (char*)calloc(sizeof(uint8_t),padding > SCAENTRY_PADDING_SIZE ? padding : SCAENTRY_PADDING_SIZE);

  // ENTRY_COUNT_TYPE entry_count = entries.size();
  // fout.write(reinterpret_cast<char *>(&entry_count), sizeof(ENTRY_COUNT_TYPE));
  // for (int i=0;i<entry_count;i++) {
  //   fout.write(reinterpret_cast<char *>(&entries[i]), SCAENTRY_SIZE);
  //   fout.write(pad_buffer, SCAENTRY_PADDING_SIZE);
  // }
  // fout.write(pad_buffer, padding);
  // free(pad_buffer);
}

bool SCA::write_file(char* path) {
  ofstream fout(path, ios::out|ios::binary);
  if (!fout.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }
  write_file(fout);
  fout.close();
  return true;
}

void SCA::cleanup() {
  entries.clear();
}

int SCA::size() {
  return entries.size();
}

uint32_t SCA::file_type() {
  return header.header;
}

SCAEntry SCA::get(int i) {
  SCAEntry entry(entries[i]);
  return entry;
}

} // namespace OKAMI_UTILS
