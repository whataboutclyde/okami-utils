#include <iostream>
#include <okami-utils/common.h>
using namespace std;

namespace OKAMI_UTILS {

bool FileReader::parse_file(fs::path path) {
  //cleanup();

  if (!fs::exists(path)) {
    cerr << "File does not exist: " << path << endl;
    return false;
  }

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  bool ret = process_file(fin);
  fin.close();
  return ret;
}

bool FileReader::parse_file(char* path) {
  return parse_file(fs::path(path));
}

void FileReader::align(ifstream& fin, int bytes) {
  int pad = fin.tellg() % bytes;
  if (pad != 0)
    fin.seekg(bytes-pad, ios::cur);
}

} // namespace OKAMI_UTILS