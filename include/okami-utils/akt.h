#ifndef OKAMI_UTILS_AKT_H
#define OKAMI_UTILS_AKT_H

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <okami-utils/ak.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

#define AKT_FILE_COUNT_TYPE     uint32_t
#define AKT_FILE_OFFSET_TYPE    uint32_t
#define AKT_PADDING_BUFFER_SIZE 32

struct AKTData {
    AKT_FILE_OFFSET_TYPE offset = 0;
//    string file_name;
    AK ak;
};

class AKT {
  private:
    static const string filelist_name;
    void cleanup();
    vector<AKTData> data;
  public:
    AKT() {};
    ~AKT();
    bool parse_file(char* path);
    bool parse_file(fs::path path);
    bool parse_file(ifstream& fin);
    // bool pack_dir(char* dir_path, char* target_path);
    int size();
    AK get(int i);
    int get_gltf_buffer_size();
    void dump_gltf_binary(ofstream& fout);
}; // class AKT

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AKT_H