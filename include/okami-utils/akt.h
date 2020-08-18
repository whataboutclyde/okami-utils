#ifndef OKAMI_UTILS_AKT_H
#define OKAMI_UTILS_AKT_H

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <okami-utils/ak.h>
using namespace std;

namespace OKAMI_UTILS {

#define AKT_FILE_COUNT_TYPE     uint32_t
#define AKT_FILE_OFFSET_TYPE    uint32_t
#define AKT_PADDING_BUFFER_SIZE 32

struct AKTData {
    AKT_FILE_OFFSET_TYPE offset = 0;
    string file_name;
    //AK ak;
};

class AKT {
  private:
    static const string filelist_name;
    void cleanup();
    vector<AKTData> data;
  public:
    AKT() {};
    AKT(char* path);
    ~AKT();
    bool unpack_file(ifstream& fin, char* path);
    bool unpack_file(char* path);
    bool pack_dir(char* dir_path, char* target_path);
    int size();
}; // class AKT

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AKT_H