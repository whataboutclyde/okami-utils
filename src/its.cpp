#include <okami-utils/its.h>
using namespace std;

namespace OKAMI_UTILS {

ITS::ITS(char* path) {
  parse_file(path);
}

ITS::~ITS() {
  cleanup();
}

bool ITS::process_file(ifstream& fin) {
  ENTRY_COUNT_TYPE entry_count = 0;
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char *>(&entry_count), sizeof(entry_count));

  for (int i=0;i<entry_count;i++) {
    ITSEntry entry;
    fin.read(reinterpret_cast<char *>(&entry), ITSENTRY_SIZE);
    entries.push_back(entry);
    fin.seekg(ITSENTRY_PADDING_SIZE, ios::cur);
  }

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
