#ifndef OKAMI_UTILS_ARCHIVE_H
#define OKAMI_UTILS_ARCHIVE_H

#include <vector>
#include <fstream>
#include <filesystem>
#include <okami-utils/common.h>
using namespace std;

namespace OKAMI_UTILS {

struct FileMetadata {
  uint32_t offset;
  uint32_t name_offset;
  uint32_t size;
  fs::path filename;
};

class GenericArchive : public FileReader {
  public:
    GenericArchive() {};
    ~GenericArchive() {};
    bool process_file(ifstream& fin);
    int size();
  private:
    uint32_t num_files;
    vector<FileMetadata> files;
};

}

#endif