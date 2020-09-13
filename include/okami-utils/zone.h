#ifndef OKAMI_UTILS_ZONE_H
#define OKAMI_UTILS_ZONE_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <filesystem>
#include <okami-utils/common.h>
using namespace std;
namespace fs = filesystem;

#define MEH_HEADER_ID 0x0048454D
#define SCA_HEADER_ID 0x00414353
#define SCI_HEADER_ID 0x00494353

namespace OKAMI_UTILS {

#define ZONE_ENTRY_SIZE sizeof(ZoneEntry)

enum ZONE_ENTRY_TYPE {
  ZONE_BIT_FLAG_TYPE = 0x00,
  ZONE_EXIT_TYPE = 0x01,
  ZONE_UNKNOWN02_TYPE = 0x02,
  ZONE_UNKNOWN03_TYPE = 0x03,
  ZONE_EXAMINE_TYPE = 0x04,
  ZONE_ISSUN_INDICATOR_TYPE = 0x05,
  ZONE_UNKNOWN06_TYPE = 0x06,
  ZONE_UNKNOWN07_TYPE = 0x07,
  ZONE_UNKNOWN08_TYPE = 0x08,
  ZONE_UNKNOWN09_TYPE = 0x09,
  ZONE_UNKNOWN0A_TYPE = 0x0A
};

enum ZONE_SHAPE {
  ZONE_SHAPE_QUAD_PRISM = 0x01,
  ZONE_SHAPE_CYLINDER = 0x02
};

#pragma pack(push, 1)
struct Point {
    float x;
    float z;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ZoneHeader {
    uint32_t header;
    uint16_t unknown1;
    uint16_t entry_count;
    uint64_t padding;       // Not actually padding in .MEH files...
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ZoneEntry {
    uint64_t pre_entry;     // seems to always be all 00.
    uint8_t always01;
    uint8_t zone_shape;
    uint8_t zero1;
    uint8_t zero2;
    float y;
    float height;
    float radius;
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

class Zone : public FileReader {
  protected:
    ZoneHeader header;
    vector<ZoneEntry> entries;
    void cleanup();
  public:
    Zone() {};
    Zone(char* path);
    ~Zone();
    bool process_file(ifstream& fin);
    int size();
    uint32_t file_type();
    ZoneEntry get(int i);
}; // class Zone

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_ZONE_H