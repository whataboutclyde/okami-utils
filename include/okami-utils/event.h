#ifndef OKAMI_UTILS_EVENT_H
#define OKAMI_UTILS_EVENT_H

#include <cstdint>
using namespace std;

#pragma pack(push, 1)
struct EventDatHeader {
    uint8_t unknown1;
    uint8_t post_header_count;
    uint8_t filename_count;
    uint8_t unknown2;
    uint32_t post_header_offset;
    uint32_t file_names_offset;
    uint32_t post_file_names_offset;
    uint32_t mtb3_data_offset;
    uint32_t unknown3[19];
    
};

struct Mtb3Entry {
    uint32_t unknown1;      // Always 00 00 00 00
    uint32_t unknown2;      // Always 00 00 00 00
    uint32_t offset;
    uint32_t unknown3;      // Always 00 00 00 00
    uint32_t alt_offset;
    uint32_t unknown4;      // Always 00 00 00 00
};
#pragma pack(pop)

#endif //OKAMI_UTILS_EVENT_H