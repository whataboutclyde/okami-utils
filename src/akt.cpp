#include <iostream>
#include <fmt/format.h>
#include <filesystem>
#include <okami-utils/akt.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

AKT::~AKT() {
  cleanup();
}

bool AKT::process_file(ifstream& fin) {
  AKT_FILE_COUNT_TYPE file_count = 0;
  fin.read(reinterpret_cast<char *>(&file_count), sizeof(file_count));

  for (int i = 0; i<file_count; i++) {
    AKTData meta;
    fin.read(reinterpret_cast<char *>(&(meta.offset)), sizeof(meta.offset));
    data.push_back(meta);
  }

  for (int i = 0; i<data.size(); i++) {
    fin.seekg(data[i].offset, ios::beg);
    data[i].ak.process_file(fin, data[i].offset);
  }

  return true;
}

void AKT::cleanup() {
  data.clear();
}

int AKT::size() {
  return data.size();
}

AK AKT::get(int i) {
  return data[i].ak;
}

} // namespace OKAMI_UTILS
