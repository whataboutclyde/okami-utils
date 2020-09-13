#ifndef OKAMI_UTILS_SCR_H
#define OKAMI_UTILS_SCR_H

#include <cstdint>
#include <fstream>
#include <filesystem>
#include <okami-utils/common.h>
#include <okami-utils/submesh.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

#define SCR_ID_STRING 0x00726373

enum {
  FILE_TYPE_MD = 0,
  FILE_TYPE_SCR = 1
};

#pragma pack(push, 1)
struct SCRHeader {
  uint32_t scr_id;          // always 00726373
  uint32_t file_type;       // 0 for md, 1 for scr?
  uint32_t end_entry_count; // For many but not all files, this is the number of submeshes.
  uint32_t padding;         // always zero
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCREndStuff {
  int16_t unknown1;
  int16_t alwaysFFFF;
  uint32_t submesh_index;
  uint32_t unknowns[11];
};
#pragma pack(pop)

class Model : public FileReader {
  public:
    Model() {};
    ~Model() {};
    bool process_file(ifstream& fin);
    int size();
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
  //private:
    SCRHeader header;
    vector<Submesh> submeshes;
    vector<SCREndStuff> end_stuff;
};

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_SCR_H