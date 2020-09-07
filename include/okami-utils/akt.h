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
    AK ak;
};

class AKT {
  protected:
    static const string filelist_name;
    void cleanup();
    vector<AKTData> data;
  public:
    AKT() {};
    ~AKT();
    bool parse_file(char* path);
    bool parse_file(fs::path path);
    bool parse_file(ifstream& fin);
    int size();
    AK get(int i);
}; // class AKT

} // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_AKT_H