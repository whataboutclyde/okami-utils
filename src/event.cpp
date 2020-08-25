#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h> 
#include "okami-utils/event_dat.h"
#include <cerrno>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        cerr << "Didn't supply file.";
        return -1;
    }

    char* path = argv[1];
    ifstream infile(path, ios::in|ios::binary|ios::ate);
    if (!infile.is_open()) {
        cerr << "Couldn't open file " << path << endl;
        cerr << strerror(errno) << endl;
        return -1;
    }

    infile.seekg(0,ios::beg);

    struct EventDatHeader header;
    infile.read(reinterpret_cast<char *>(&header), sizeof(header));

    cout << "file: " << path << endl;
    // cout << "unknown1: " << std::hex << unsigned(header.unknown1) << endl;
    // cout << "post_header_count: " << std::dec << unsigned(header.post_header_count) << endl;
    // cout << "filename_count: " << std::dec << unsigned(header.filename_count) << endl;
    // cout << "unknown2: " << std::hex << unsigned(header.unknown2) << endl;
    // cout << "post_header_offset: " << std::hex << header.post_header_offset << endl;
    // cout << "file_names_offset: " << std::hex << header.file_names_offset << endl;
    // cout << "post_file_names_offset: " << std::hex << header.post_file_names_offset << endl;

    if (header.post_header_offset != 0x60)
        cout << "post_header_offset expected 0x60, got: " << std::hex << header.post_header_offset << endl;

    // uint32_t post_header_size = header.file_names_offset - header.post_header_offset;
    // uint32_t estimated_size = header.post_header_count * 24;
    // estimated_size += (estimated_size%16);
    
    // cout << "post_header_size: " << post_header_size << endl;
    // cout << "data size: " << header.post_header_count*24 << endl;
    // cout << "% 16: " << (header.post_header_count*24)%16 << endl;
    // cout << "% 32: " << (header.post_header_count*24)%32 << endl;
    // cout << "estimated_size: " << estimated_size << endl;

    // if (post_header_size != estimated_size) {
    //     cout << "Wrong assumption. Actual size: " << post_header_size << "; estimated size: " << estimated_size << endl;
    //     cout << "data size: " << header.post_header_count*24 << endl;
    // }

    struct Mtb3Entry* mtb3_entries = (struct Mtb3Entry*)calloc(header.post_header_count, sizeof(*mtb3_entries));
    infile.seekg(header.post_header_offset, ios::beg);
    infile.read(reinterpret_cast<char *>(mtb3_entries), header.post_header_count*sizeof(*mtb3_entries));

    // if (header.mtb3_data_offset != mtb3_entries[0].offset &&
    //     header.mtb3_data_offset != mtb3_entries[0].alt_offset)
    //     cout << "I fucked up." << endl;

    // for (int i=0; i<header.post_header_count; i++) {
    //     if (mtb3_entries[i].offset != 0)
    //         cout << "mtb3 offset: " << std::hex << mtb3_entries[i].offset << endl;
    //     else if (mtb3_entries[i].alt_offset != 0)
    //         cout << "alt offset:  " << std::hex << mtb3_entries[i].alt_offset << endl;
    //     else
    //         cout << "I fucked up." << endl;

    //     if (mtb3_entries[i].offset != 0 && mtb3_entries[i].alt_offset != 0)
    //         cout << "I REALLY fucked up." << endl;
        

    //     if (mtb3_entries[i].unknown1 != 0x00)
    //         cout << "unknown1: " << std::hex << mtb3_entries[i].unknown1 << endl;
    //     if (mtb3_entries[i].unknown2 != 0x00)
    //         cout << "unknown2: " << std::hex << mtb3_entries[i].unknown2 << endl;
    //     if (mtb3_entries[i].unknown3 != 0x00)
    //         cout << "unknown3: " << std::hex << mtb3_entries[i].unknown3 << endl;
    //     if (mtb3_entries[i].unknown4 != 0x00)
    //         cout << "unknown4: " << std::hex << mtb3_entries[i].unknown4 << endl;
    // }

    infile.close();
}