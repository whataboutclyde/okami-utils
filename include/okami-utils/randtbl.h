#ifndef OKAMI_UTILS_RANDTBL_H
#define OKAMI_UTILS_RANDTBL_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <okami-utils/common.h>
using namespace std;

namespace OKAMI_UTILS {

#define RANDTBLGROUPHEADER_SIZE sizeof(RANDTBLGroupHeader)
#define RANDTBLENTRY_SIZE sizeof(RANDTBLEntry)
#define RANDTBL_ENTRY_COUNT_TYPE uint32_t

#pragma pack(push, 1)
struct RANDTBLGroupHeader {
  uint8_t drop_chance; // Likelihood you get a drop.
  uint8_t count;
  uint8_t total_weight; // Total weight for drop_list.
  uint8_t pad; // Always 0x00
};
#pragma pack(pop)

#pragma pack(push, 1)
struct RANDTBLEntry {
  uint8_t obj_id; 
  uint8_t cat_id; // which folder, i.e. 0A is /it/, and in all files, all entries use 0A.
  uint8_t drop_weight; // Sum all drop_weight in a group and it equals total_weight of the header.
  uint8_t drop_count; // How many items drop.
};
#pragma pack(pop)

struct RANDTBLGroup {
  RANDTBLGroupHeader header;
  vector<RANDTBLEntry> entries;
};

class RANDTBL : public FileReader {
  private:
    vector<RANDTBLGroup> groups;
    void cleanup();
  public:
    RANDTBL() {};
    RANDTBL(char* path);
    ~RANDTBL();
    bool process_file(ifstream& fin);
    int size();
    RANDTBLGroup get(int i);
}; // class RANDTBL

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_RANDTBL_H