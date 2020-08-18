#include <okami-utils/its.h>
using namespace std;

namespace OKAMI_UTILS {

ITS::ITS(char* path) {
  parse_file(path);
}

ITS::~ITS() {
  cleanup();
}

void ITS::parse_file(ifstream& fin) {
  ENTRY_COUNT_TYPE entry_count = 0;
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char *>(&entry_count), sizeof(entry_count));

  for (int i=0;i<entry_count;i++) {
    ITSEntry entry;
    fin.read(reinterpret_cast<char *>(&entry), ITSENTRY_SIZE);
    entries.push_back(entry);
    fin.seekg(ITSENTRY_PADDING_SIZE, ios::cur);
  }
}

bool ITS::parse_file(char* path) {
  cleanup();

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  parse_file(fin);
  fin.close();
  return true;
}

void ITS::write_file(ofstream& fout) {
  int file_size = sizeof(ENTRY_COUNT_TYPE) + (ITSENTRY_SIZE + ITSENTRY_PADDING_SIZE)*entries.size();
  int padding = 32-(file_size % 32); // padding for 32 byte alignment.
  char* pad_buffer = (char*)calloc(sizeof(uint8_t),padding > ITSENTRY_PADDING_SIZE ? padding : ITSENTRY_PADDING_SIZE);

  ENTRY_COUNT_TYPE entry_count = entries.size();
  fout.write(reinterpret_cast<char *>(&entry_count), sizeof(ENTRY_COUNT_TYPE));
  for (int i=0;i<entry_count;i++) {
    fout.write(reinterpret_cast<char *>(&entries[i]), ITSENTRY_SIZE);
    fout.write(pad_buffer, ITSENTRY_PADDING_SIZE);
  }
  fout.write(pad_buffer, padding);
  free(pad_buffer);
}

bool ITS::write_file(char* path) {
  ofstream fout(path, ios::out|ios::binary);
  if (!fout.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }
  write_file(fout);
  fout.close();
  return true;
}

void ITS::cleanup() {
  entries.clear();
}

int ITS::size() {
  return entries.size();
}

ITSEntry ITS::get(int i) {
  ITSEntry entry(entries[i]);
  return entry;
}

} // namespace OKAMI_UTILS
