#ifndef OKAMI_UTILS_ITS_H
#define OKAMI_UTILS_ITS_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <okami-utils/common.h>
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
  UInt8Tuple size;
  UInt8Tuple rotation;
  Int16Tuple coordinates;
  uint8_t container_type;
  uint8_t unknowns[NUM_UNKNOWNS];
  //ITSENTRY_PADDING_SIZE bytes padding, just fseek past it.
};
#pragma pack(pop)

class ITS : FileReader {
  private:
    vector<ITSEntry> entries;
    void cleanup();
  public:
    ITS() {};
    ITS(char* path);
    ~ITS();
    bool process_file(ifstream& fin);
    // void write_file(ofstream& fout);
    // bool write_file(char* path);
    int size();
    ITSEntry get(int i);
}; // class ITS

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_ITS_H