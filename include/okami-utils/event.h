#ifndef OKAMI_UTILS_EVENT_H
#define OKAMI_UTILS_EVENT_H

#include <cstdint>
using namespace std;

namespace OKAMI_UTILS {

#define EVENT_ARCHIVE_ID  0x1
#define EVENT_ARCHIVE_EOH 0x60

#define EFF_ARCHIVE_COUNT_SIZE uint32_t
static const char SEQ_HEADER[4] = "SEQ";
static const char MOT_HEADER[5] = "mtb3";
static const char EV_HEADER[3] = "EV"; // EVE, EVF, EVL
static const uint64_t EVENT_MTB3_GREEN_HEADER = 0x0605040302010000;
static const uint64_t EVENT_MTB3_GREEN_HEADER_MASK = 0xFFFFFFFFFFFFFF00;

#pragma pack(push, 1)
struct EventArchiveHeader {
    uint8_t file_type_id; // always 01
    uint8_t mtb3_entry_count;
    uint8_t obj_string_count;
    uint8_t pad;
    uint32_t end_header_offset;
    uint32_t obj_names_offset;
    uint32_t more_mtb3_offset;
    uint32_t mtb3_data_offset;
    uint32_t other_offsets[19]; // SEQ or size for EFF
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Mtb3BlueEntry {
    uint32_t unknown1;      // Always 00 00 00 00
    uint32_t unknown2;      // Always 00 00 00 00
    uint32_t offset;
    uint32_t unknown3;      // Always 00 00 00 00
    uint32_t alt_offset;
    uint32_t unknown4;      // Always 00 00 00 00
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Mtb3GreenEntry{
  uint64_t header;
  uint64_t offsets[6];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ObjectIDEntry {
  char file_id[4];
  uint8_t index;
  uint8_t padding[3];
};
#pragma pack(pop)

}; // namespace OKAMI_UTILS

#endif //OKAMI_UTILS_EVENT_H