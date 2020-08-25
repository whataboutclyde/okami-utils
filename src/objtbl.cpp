#include <okami-utils/objtbl.h>
using namespace std;

namespace OKAMI_UTILS {

OBJTBL::OBJTBL(char* path) {
  parse_file(path);
}

OBJTBL::~OBJTBL() {
  cleanup();
}

void OBJTBL::parse_file(ifstream& fin) {
  OBJTBL_ENTRY_COUNT_TYPE entry_count = 0;
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char *>(&entry_count), sizeof(entry_count));

  if (entry_count == 0)
    return;

  for (int i=0;i<entry_count;i++) {
    OBJTBLEntry entry;
    fin.read(reinterpret_cast<char *>(&entry), OBJTBLENTRY_SIZE);
    entries.push_back(entry);
    fin.seekg(OBJTBL_ENTRY_PADDING_SIZE, ios::cur);
  }
}

bool OBJTBL::parse_file(char* path) {
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

void OBJTBL::write_file(ofstream& fout) {
  int file_size = sizeof(OBJTBL_ENTRY_COUNT_TYPE) + (OBJTBLENTRY_SIZE + OBJTBL_ENTRY_PADDING_SIZE)*entries.size();
  int padding = 32-(file_size % 32); // padding for 32 byte alignment.
  char* pad_buffer = (char*)calloc(sizeof(uint8_t),padding > OBJTBL_ENTRY_PADDING_SIZE ? padding : OBJTBL_ENTRY_PADDING_SIZE);

  OBJTBL_ENTRY_COUNT_TYPE entry_count = entries.size();
  fout.write(reinterpret_cast<char *>(&entry_count), sizeof(OBJTBL_ENTRY_COUNT_TYPE));
  for (int i=0;i<entry_count;i++) {
    fout.write(reinterpret_cast<char *>(&entries[i]), OBJTBLENTRY_SIZE);
    fout.write(pad_buffer, OBJTBL_ENTRY_PADDING_SIZE);
  }
  fout.write(pad_buffer, padding);
  free(pad_buffer);
}

bool OBJTBL::write_file(char* path) {
  ofstream fout(path, ios::out|ios::binary);
  if (!fout.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }
  write_file(fout);
  fout.close();
  return true;
}

void OBJTBL::cleanup() {
  entries.clear();
}

int OBJTBL::size() {
  return entries.size();
}

OBJTBLEntry OBJTBL::get(int i) {
  OBJTBLEntry entry(entries[i]);
  return entry;
}

} // namespace OKAMI_UTILS
