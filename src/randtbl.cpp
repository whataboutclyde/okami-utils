#include <okami-utils/randtbl.h>
using namespace std;

namespace OKAMI_UTILS {

RANDTBL::RANDTBL(char* path) {
  parse_file(path);
}

RANDTBL::~RANDTBL() {
  cleanup();
}

bool RANDTBL::process_file(ifstream& fin) {
  RANDTBL_ENTRY_COUNT_TYPE entry_count = 0;
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char *>(&entry_count), sizeof(entry_count));

  if (entry_count == 0)
    return false;

  for (int i=0;i<entry_count;i++) {
    RANDTBLGroup group;
    fin.read(reinterpret_cast<char *>(&(group.header)), RANDTBLGROUPHEADER_SIZE);
    for (int j=0; j<group.header.count;j++) {
      RANDTBLEntry entry;
      fin.read(reinterpret_cast<char *>(&entry), RANDTBLENTRY_SIZE);
      group.entries.push_back(entry);
    }
    groups.push_back(group);
  }

  while (!fin.eof()) {
    char c;
    fin.get(c);
    if (c != 0) {
      cout << "FUCK" << endl;
      return false;
    }
  }

  return true;
}

void RANDTBL::cleanup() {
  groups.clear();
}

int RANDTBL::size() {
  return groups.size();
}

RANDTBLGroup RANDTBL::get(int i) {
  RANDTBLGroup entry(groups[i]);
  return entry;
}

} // namespace OKAMI_UTILS
