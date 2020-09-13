#include <okami-utils/objtbl.h>
using namespace std;

namespace OKAMI_UTILS {

OBJTBL::OBJTBL(char* path) {
  parse_file(path);
}

OBJTBL::~OBJTBL() {
  cleanup();
}

bool OBJTBL::process_file(ifstream& fin) {
  OBJTBL_ENTRY_COUNT_TYPE entry_count = 0;
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char *>(&entry_count), sizeof(entry_count));

  if (entry_count == 0)
    return false;

  for (int i=0;i<entry_count;i++) {
    OBJTBLEntry entry;
    fin.read(reinterpret_cast<char *>(&entry), OBJTBLENTRY_SIZE);
    entries.push_back(entry);
    fin.seekg(OBJTBL_ENTRY_PADDING_SIZE, ios::cur);
  }

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
