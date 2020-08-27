#include <iostream>
#include <fmt/format.h>
#include <filesystem>
#include <okami-utils/akt.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

AKT::~AKT() {
  cleanup();
}

bool AKT::parse_file(char* path) {
  return parse_file(fs::path(path));
}

bool AKT::parse_file(fs::path path) {
  cleanup();

  if (!fs::exists(path)) {
    cerr << "File does not exist: " << path << endl;
    return false;
  }

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  bool ret = parse_file(fin);
  fin.close();
  return ret;
}


// TODO add error handling.
bool AKT::parse_file(ifstream& fin) {
  AKT_FILE_COUNT_TYPE file_count = 0;
  fin.read(reinterpret_cast<char *>(&file_count), sizeof(file_count));

  for (int i = 0; i<file_count; i++) {
    AKTData meta;
    fin.read(reinterpret_cast<char *>(&(meta.offset)), sizeof(meta.offset));
    data.push_back(meta);
  }

  for (int i = 0; i<file_count; i++) {
    fin.seekg(data[i].offset, ios::beg);
    data[i].ak.parse_file(fin, data[i].offset);
  }

  return true;
}

// bool AKT::pack_dir(char* dir_path, char* target_path) {
//   if (!fs::exists(dir_path)) {
//     cerr << dir_path << " does not exist." << endl;
//     return false;
//   }

//   ifstream fin(fmt::format("{}/{}", dir_path, filelist_name), ios::in|ios::binary);
//   if (!fin.is_open()) {
//     cerr << "Failed to read file list: " << dir_path << "/" << filelist_name << endl;
//     return false;
//   }

//   ofstream fout(target_path, ios::out|ios::binary);
//   if (!fout.is_open()) {
//     cerr << "Failed to open file: " << target_path << endl;
//     return false;
//   }

//   AKT_FILE_COUNT_TYPE num_files = 0;
//   fin.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
//   fout.write(reinterpret_cast<char*>(&num_files), sizeof(num_files));

//   if (num_files == 0) {
//     fin.close();
//     fout.close();
//     return true;
//   }

//   vector<AKTData> meta;
//   // ngl this first_offset shit is dumb. It's always 0, and 0-3 is 0x68, 4-7 is 0x88, 8-11 is 0xa8 etc.
//   // So start at 0x68 and for every 4, add 32.
//   // Makes no sense to me.
//   AKT_FILE_OFFSET_TYPE first_offset = 0x68+(num_files/4)*32;
//   AKT_FILE_OFFSET_TYPE offset = first_offset;
//   char padding_buffer[AKT_PADDING_BUFFER_SIZE];
//   memset(padding_buffer, 0, AKT_PADDING_BUFFER_SIZE);

//   for (int i=0; i<num_files; i++) {
//     AKTData m;
//     fin >> m.file_name;
//     m.offset = offset;
//     fout.write(reinterpret_cast<char*>(&(m.offset)), sizeof(m.offset));
//     offset += fs::file_size(fs::path{fmt::format("{}/{}", dir_path, m.file_name)});
//     meta.push_back(m);
//     //cout << hex << setfill('0') << setw(8) << m.offset << " " << m.file_name << endl;
//   }
//   fin.close();

//   // write extensions.
//   for (int i=0; i<num_files; i++) {
//     fs::path fpath{meta[i].file_name};
//     fs::path ext{fpath.extension()};
//     string ext_s = ext.string().substr(1,ext.string().length()-1);
//     fout << ext_s;
//     fout.write(padding_buffer, 4-(ext_s.length()%4));
//   }

//   // Weird padding.
//   int padding_size=first_offset-fout.tellp();
//   while (padding_size > 0) {
//     int write_size = padding_size < AKT_PADDING_BUFFER_SIZE ? padding_size : AKT_PADDING_BUFFER_SIZE;
//     fout.write(padding_buffer, write_size);
//     padding_size -= write_size;
//   }

//   // Append all the files.
//   for (int i=0; i<num_files; i++) {
//     fin.open(fs::path{fmt::format("{}/{}", dir_path, meta[i].file_name)}, ios::in|ios::binary);
//     fout << fin.rdbuf();
//     fin.close();
//   }

//   fout.close();

//   return true;
// }

void AKT::cleanup() {
  data.clear();
}

int AKT::size() {
  return data.size();
}

AK AKT::get(int i) {
  return data[i].ak;
}

} // namespace OKAMI_UTILS
