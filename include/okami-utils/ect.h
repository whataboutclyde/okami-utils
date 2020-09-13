#ifndef OKAMI_UTILS_ECT_H
#define OKAMI_UTILS_ECT_H

#include <fstream>
#include <filesystem>
#include <okami-utils/common.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

#define ECT_ENTRY_HEADER_TYPE uint32_t

#define ECT_FLAGS_TOD_MASK 0x30
#define ECT_FLAGS_TOD_DAY_ONLY 0x10
#define ECT_FLAGS_TOD_NIGHT_ONLY 0x20

#define ECT_FLAGS_ESCAPE_MASK     (0x40)
#define ECT_FLAGS_INESCAPABLE     (0x40)

#pragma pack(push, 1)
struct EnemyEntry {
  uint8_t enemy_id;
  uint8_t cat_id; // Always 02 (em folder).
  uint8_t unknowns[6];
  uint8_t total; // 00-0a, 0c, 0e
  uint8_t max_per_wave; // 00-06, 08
  uint8_t unknown1; // always 00
  uint8_t unknown2; //always 0A in shinshu, else: 0d, 32, 3c
  uint8_t padding[12]; // always 00
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ECTEntry {
  Int16Tuple coordinates;
  uint8_t unknown1; // 00, 03-05, 07, 08, fe
  uint8_t escape_radius;
  uint8_t flags; // 01, 09, 11, 21, 61, 81, 83, 85, e1
  uint8_t type; //04 scroll, 09 gate, bunch of others?
  int16_t pad;
  uint16_t index; // Increasing, no dupes in file, some missing.
  uint16_t unknown3; // 00-1f
  int32_t pad2;
  EnemyEntry enemy[4];
  uint8_t unknown4; // 00-02, 0c
  uint8_t unknown5; //00
  uint8_t unknown6; // 00, 07
  uint8_t unknownugh; // 00-06
  uint8_t unknown7; // bunch of values
  uint8_t unknown8; // 00-04
  uint8_t unknown9; // 00, 01, 03
  uint8_t unknowna; // 00-07
  uint8_t mix_enemies; // 00 mix, 01 separate different enemy types to different waves
  uint8_t padding[7];
};
#pragma pack(pop)

class ECT : public FileReader {
  public:
    ECT() {};
    ~ECT() {};
    bool process_file(ifstream& fin);
    int size();
    ECTEntry get(int i);
  private:
    vector<ECTEntry> entries;
};

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_ECT_H