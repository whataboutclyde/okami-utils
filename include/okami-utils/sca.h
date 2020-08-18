#ifndef OKAMI_UTILS_SCA_H
#define OKAMI_UTILS_SCA_H

#include <cstdint>
#include <vector>
#include <fstream>
using namespace std;

#define MEH_HEADER_ID 0x0048454D
#define SCA_HEADER_ID 0x00414353
#define SCI_HEADER_ID 0x00494353

namespace OKAMI_UTILS {

#define SCA_ENTRY_SIZE sizeof(SCAEntry)

#pragma pack(push, 1)
struct Point {
    float x;
    float z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCAHeader {
    uint32_t header;
    uint16_t unknown1;
    uint16_t entry_count;
    uint64_t padding;       // Not actually padding in .MEH files...
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SCAEntry {
    uint64_t pre_entry;     // seems to always be all 00.
    uint32_t header;
    float y;
    float height;
    float unknown_float;
    Point p1;
    Point p2;
    Point p3;
    Point p4;
    uint8_t unknown2;
    uint8_t entry_type;
    uint16_t effect_index;
    uint8_t unknown3;
    uint8_t unknown4;
    uint8_t unknown5;
    uint8_t unknown6;
    uint8_t unknown_data[112];
};
#pragma pack(pop)

class SCA {
  private:
    SCAHeader header;
    vector<SCAEntry> entries;
    void cleanup();
  public:
    SCA() {};
    SCA(char* path);
    ~SCA();
    bool parse_file(ifstream& fin);
    bool parse_file(char* path);
    void write_file(ofstream& fout);
    bool write_file(char* path);
    int size();
    uint32_t file_type();
    SCAEntry get(int i);
}; // class SCA

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_SCA_H