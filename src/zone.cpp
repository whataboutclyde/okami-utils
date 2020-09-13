#include <iostream>
#include <okami-utils/zone.h>
using namespace std;

namespace OKAMI_UTILS {

Zone::Zone(char* path) {
  parse_file(path);
}

Zone::~Zone() {
  cleanup();
}

bool Zone::process_file(ifstream& fin) {
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char*>(&header),sizeof(header));

  if (header.header != MEH_HEADER_ID && header.header != SCA_HEADER_ID && header.header != SCI_HEADER_ID) {
    cerr << "Not a valid Zone file. Header: " << hex << header.header << endl;
    return false;
  }

  for (int i=0; i<header.entry_count;i++) {
    ZoneEntry entry;
    fin.read(reinterpret_cast<char*>(&entry), ZONE_ENTRY_SIZE);

    entries.push_back(entry);
  }

  return true;
}

void Zone::cleanup() {
  entries.clear();
}

int Zone::size() {
  return entries.size();
}

uint32_t Zone::file_type() {
  return header.header;
}

ZoneEntry Zone::get(int i) {
  ZoneEntry entry(entries[i]);
  return entry;
}

} // namespace OKAMI_UTILS
