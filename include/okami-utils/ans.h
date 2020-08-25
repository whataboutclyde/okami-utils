#ifndef OKAMI_UTILS_ANS_H
#define OKAMI_UTILS_ANS_H

#include <cstdint>
#include <map>
#include <list>
#include <vector>
#include <fstream>
using namespace std;

namespace OKAMI_UTILS {

#define ANS_ENTRY_COUNT_TYPE uint32_t
#define ANS_ENTRY_SIZE sizeof(ANSEntry)
#define ANS_ENTRY_PADDING_SIZE 12

enum {
  TOD_ALWAYS = 0x00,
  TOD_DAY_ONLY = 0x01,
  TOD_NIGHT_ONLY = 0x02
};

#pragma pack(push, 1)
struct ANSEntry {
  uint8_t an_id;    // id from an folder
  uint8_t cat_id;   // category id; always 05
  uint8_t unknown2; // always 03
  uint8_t unknown3; // always 00
  uint8_t x_size;
  uint8_t y_size;
  uint8_t z_size;
  uint8_t x_rotate;
  uint8_t y_rotate;
  uint8_t z_rotate;
  int16_t x_coord;
  int16_t y_coord;
  int16_t z_coord;
  uint8_t unknown4; // 00-04, seems to affect animal texture used.
  uint8_t unknown5; // 00-08, 0f
  uint8_t group_id; // Offset for the group fed bit in memory starting at main.dll+B213AE bit 3? (First group is 03, at bit 6.)
  uint8_t tod;      // See TOD_* enum above.
  uint8_t unknown6; // always 00
  uint8_t unknown7; // always 00
  uint8_t an_id2;   // Always matches an_id, doesn't seem used for anything
  uint8_t unknown8; // always 00
  uint8_t unknown9; // 00-08, 0f, 10, ff
  uint8_t unknowna; // 00-03
  uint8_t unknownb; // only used for single monkey at rf0a: 50. Otherwise 00
  uint8_t unknownc; // only used for single monkey at rf0a: c3. Otherwise 00.
  uint8_t padding[ANS_ENTRY_PADDING_SIZE];
};
#pragma pack(pop)

class ANS {
  private:
    list<uint8_t> group_ids;
    multimap<uint8_t, ANSEntry> groups;
    //vector<ANSEntry> entries;
    void cleanup();
  public:
    ANS() {};
    ANS(char* path);
    ~ANS();
    bool parse_file(ifstream& fin);
    bool parse_file(char* path);
    void write_file(ofstream& fout);
    bool write_file(char* path);
    int size();
    list<uint8_t> get_keys();
    vector<ANSEntry> get(uint8_t i);
}; // class ANS

} // namespace OKAMI_UTILS

#endif // OKAMI_UTILS_ANS_H