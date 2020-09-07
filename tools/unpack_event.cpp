#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <okami-utils/event.h>
using namespace std;
namespace fs = filesystem;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Need .dat file arg." << endl;
    return -1;
  }

  fs::path path(argv[1]);

  if (!fs::exists(path) || path.extension() != ".dat") {
    cerr << "Are you fucking with me?" << endl;
    return -1;
  }

  size_t file_size = fs::file_size(path);
  fs::path out_dir(path);
  out_dir += "_dir";
  fs::create_directory(out_dir);

  ifstream fin(path, ios::in|ios::binary);
  OKAMI_UTILS::EventArchiveHeader header;
  vector<OKAMI_UTILS::Mtb3BlueEntry> blues;
  vector<OKAMI_UTILS::ObjectIDEntry> ids;
  vector<OKAMI_UTILS::Mtb3GreenEntry> greens;
  vector<uint32_t> offsets;

  fin.read(reinterpret_cast<char*>(&header), sizeof(header));

  if (header.file_type_id != EVENT_ARCHIVE_ID) {
    cerr << "This isn't an event .dat file." << endl;
    fin.close();
    return -1;
  }

  cout << path << endl;
  // cout << "strings: " << dec << unsigned(header.obj_string_count) << endl;
  // cout << "mtb3: " << dec << unsigned(header.mtb3_entry_count) << endl;
  // cout << "green entries: " << (header.mtb3_data_offset-header.more_mtb3_offset)/56 << endl;

  if (header.obj_string_count*header.mtb3_entry_count != (header.mtb3_data_offset-header.more_mtb3_offset)/56)
    cout << "FUCK ME" << endl;

  if (header.end_header_offset + 24*header.mtb3_entry_count > header.obj_names_offset)
    cout << "HA THIS IS WRONG NEW THEORY IS RIGHT" << endl;

  // Read in blue..
  fin.seekg(header.end_header_offset, ios::beg);
  for (int i = 0; i < header.mtb3_entry_count; i++) {
    OKAMI_UTILS::Mtb3BlueEntry blue;
    fin.read(reinterpret_cast<char*>(&blue), sizeof(blue));
    if (blue.unknown1!=0)
      cout << "unknown1: " << hex << setfill('0') << setw(8) << blue.unknown1 << endl;
    if (blue.unknown2!=0)
      cout << "unknown1: " << hex << setfill('0') << setw(8) << blue.unknown2 << endl;
    if (blue.unknown3!=0)
      cout << "unknown1: " << hex << setfill('0') << setw(8) << blue.unknown3 << endl;
    if (blue.unknown4!=0)
      cout << "unknown1: " << hex << setfill('0') << setw(8) << blue.unknown4 << endl;

    if (blue.offset == 0 && blue.alt_offset == 0)
      cout << "FUCK both offsets 0" << endl;
    else if (blue.offset != 0 && blue.alt_offset != 0)
      cout << "FUCK neither offsets 0: m " << hex << setfill('0') << setw(8) << blue.offset << "; a " << blue.alt_offset << endl;
    blues.push_back(blue);

    if (blue.offset != 0)
      offsets.push_back(blue.offset);
    else
      offsets.push_back(blue.alt_offset);
  }

  // int padding = header.obj_names_offset - (header.end_header_offset+header.mtb3_entry_count*sizeof(OKAMI_UTILS::Mtb3BlueEntry));
  // for (int i=0; i<padding; i++) {
  //   char c;
  //   fin.get(c);
  //   if (c != '\0')
  //     cout << "NOT PADDING!!! " << unsigned(c) << endl;
  // }

  fin.seekg(header.obj_names_offset, ios::beg);
  for (int i=0; i<header.obj_string_count; i++) {
    OKAMI_UTILS::ObjectIDEntry id;
    fin.read(reinterpret_cast<char*>(&id), sizeof(id));
    ids.push_back(id);
    // char s[5];
    // strncpy(s,id.file_id,4);
    // s[4] = '\0';
    // cout << s << "(" << hex << unsigned(id.index) << ")" << endl;
    // for (int i=0;i<3;i++) {
    //   if (id.padding[i] != 0)
    //     cout << "NOT PADDING " << id.file_id << " " << unsigned(id.index) << " [" << i << "]: " << hex << unsigned(id.padding[i]) << endl;
    // }
  }
  
  // int padding = header.more_mtb3_offset - (header.obj_names_offset+header.obj_string_count*sizeof(OKAMI_UTILS::ObjectIDEntry));
  // for (int i=0; i<padding; i++) {
  //   char c;
  //   fin.get(c);
  //   if (c != '\0')
  //     cout << "NOT PADDING!!! " << unsigned(c) << endl;
  // }

  fin.seekg(header.more_mtb3_offset, ios::beg);
  for (int i=0; i<header.obj_string_count*header.mtb3_entry_count; i++) {
    OKAMI_UTILS::Mtb3GreenEntry green;
    fin.read(reinterpret_cast<char*>(&green), sizeof(green));
    uint64_t masked_header = green.header&OKAMI_UTILS::EVENT_MTB3_GREEN_HEADER_MASK;
    if (masked_header != OKAMI_UTILS::EVENT_MTB3_GREEN_HEADER)
      cout << "SOMETHING WENT WRONG " << hex << setfill('0') << setw(16) << masked_header << endl;
    greens.push_back(green);
    for (int i=0; i<6; i++) {
      if (green.offsets[i] != 0)
        offsets.push_back(green.offsets[i]);
    }
  }

  for (int i=0; i<19; i++) {
    if (header.other_offsets[i] != 0)
      offsets.push_back(header.other_offsets[i]);
  }

  for (int i=0; i<offsets.size(); i++) {
    fin.seekg(offsets[i], ios::beg);
    int size = i==offsets.size()-1 ? file_size - offsets[i] : offsets[i+1] - offsets[i];
    char* buffer = (char*)malloc(size);
    fin.read(buffer,size);

    fs::path out(out_dir);
    char c[4];
    out /= itoa(i, c, 10);
    if (strncmp(buffer, OKAMI_UTILS::SEQ_HEADER, 4) == 0)
      out += ".SEQ";
    else if (strncmp(buffer, OKAMI_UTILS::MOT_HEADER, 4) == 0)
      out += ".MOT";
    else if (strncmp(buffer, OKAMI_UTILS::EV_HEADER, 2) == 0)
      out += ".EV";
    else
      out += ".EFF";
    // Hopefully this is all the sorts... I don't actually know.

    ofstream fout(out, ios::out|ios::binary);
    fout.write(buffer, size);
    fout.close();
    free(buffer);
  }

  fin.close();

  return 0;
}