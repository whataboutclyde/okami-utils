#ifndef OKAMI_UTILS_ITS_H
#define OKAMI_UTILS_ITS_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

namespace OKAMI_UTILS {

#define ITSENTRY_PADDING_SIZE 15
#define NUM_UNKNOWNS 23 - ITSENTRY_PADDING_SIZE
#define ITSENTRY_SIZE sizeof(ITSEntry)
#define ENTRY_COUNT_TYPE uint32_t

#pragma pack(push, 1)
struct ITSEntry {
  uint8_t contents_id;
  uint8_t unknown1; // always 0a
  uint8_t unknown2; // always 01
  uint8_t container_state;
  uint8_t x_size;
  uint8_t y_size;
  uint8_t z_size;
  uint8_t x_rotate;
  uint8_t y_rotate;
  uint8_t z_rotate;
  int16_t x_coord;
  int16_t y_coord;
  int16_t z_coord;
  uint8_t container_type;
  uint8_t unknowns[NUM_UNKNOWNS];
  //ITSENTRY_PADDING_SIZE bytes padding, just fseek past it.
};
#pragma pack(pop)

class ITS {
  private:
    vector<ITSEntry> entries;
    void cleanup();
  public:
    ITS() {};
    ITS(char* path);
    ~ITS();
    void parse_file(ifstream& fin);
    bool parse_file(char* path);
    void write_file(ofstream& fout);
    bool write_file(char* path);
    int size();
    ITSEntry get(int i);
}; // class ITS

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_ITS_H