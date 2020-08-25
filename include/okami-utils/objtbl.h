#ifndef OKAMI_UTILS_OBJTBL_H
#define OKAMI_UTILS_OBJTBL_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

namespace OKAMI_UTILS {

#define OBJTBL_ENTRY_PADDING_SIZE 12
#define OBJTBL_NUM_UNKNOWNS 12-OBJTBL_ENTRY_PADDING_SIZE
#define OBJTBLENTRY_SIZE sizeof(OBJTBLEntry)
#define OBJTBL_ENTRY_COUNT_TYPE uint32_t

#pragma pack(push, 1)
struct OBJTBLEntry {
  uint8_t obj_id; // Lots of values 00-fe 
  uint8_t cat_id; // category id - which folder it comes from
  uint8_t unknown3; // 00, 10, 20, 30, 40, 50, 60, 70, 80, 90
  uint8_t unknown4; // always 00
  uint8_t x_size;
  uint8_t y_size;
  uint8_t z_size;
  uint8_t x_rotate;
  uint8_t y_rotate;
  uint8_t z_rotate;
  int16_t x_coord;
  int16_t y_coord;
  int16_t z_coord;
  uint32_t unknown5; // a lot of values, only first byte is used.
  uint32_t unknown6; // a lot of values, only first byte is used.
  uint8_t unknown7; // 00-0c
  uint8_t unknown8; // lots of values
  uint8_t unknown9; // bunch, 0*, 4* or 70 (not all values of * though)
  uint8_t unknowna; // lots of values
  uint8_t unknowns[OBJTBL_NUM_UNKNOWNS];
};
#pragma pack(pop)

class OBJTBL {
  private:
    vector<OBJTBLEntry> entries;
    void cleanup();
  public:
    OBJTBL() {};
    OBJTBL(char* path);
    ~OBJTBL();
    void parse_file(ifstream& fin);
    bool parse_file(char* path);
    void write_file(ofstream& fout);
    bool write_file(char* path);
    int size();
    OBJTBLEntry get(int i);
}; // class OBJTBL

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_OBJTBL_H