#include <iostream>
#include <fmt/format.h>
#include <filesystem>
#include <okami-utils/akt.h>
using namespace std;
namespace fs = filesystem;

namespace OKAMI_UTILS {

const string AKT::filelist_name = "filelist.lst";

AKT::AKT(char* path) {
  unpack_file(path);
}

AKT::~AKT() {
  cleanup();
}

bool AKT::unpack_file(ifstream& fin, char* path) {
  AKT_FILE_COUNT_TYPE file_count = 0;
  int32_t total_size = fs::file_size(path);
  fin.read(reinterpret_cast<char *>(&file_count), sizeof(file_count));

  for (int i = 0; i<file_count; i++) {
    AKTData meta;
    fin.read(reinterpret_cast<char *>(&(meta.offset)), sizeof(meta.offset));
    data.push_back(meta);
  }

  for (int i = 0; i<file_count; i++) {
    string ext;
    getline(fin, ext, '\0');
    data[i].file_name.resize(10);
    data[i].file_name=fmt::format("{}.{}", i, ext);
    fin.seekg(4 - ((fin.tellg()) % 4), ios::cur);
  }

  string dir_path = fmt::format("{}_dir/", path);
  if (fs::create_directory(dir_path) == -1 && errno != EEXIST) {
    cerr << "Failed to make directory: " << dir_path << endl << strerror(errno) << endl;
    return -1;
  }

  ofstream filelist(dir_path+filelist_name, ios::out|ios::binary);
  if (!filelist.is_open()) {
    cerr << "Failed to open file: " << dir_path << filelist_name << endl;
    return -1;
  }

  filelist.write(reinterpret_cast<char*>(&file_count), sizeof(file_count));
  for (int i = 0; i<file_count; i++) {
    string file_name=dir_path+data[i].file_name;

    ofstream fout(file_name, ios::out|ios::binary|ios::trunc);
    if (!fout.is_open()) {
      cerr << "Failed to open file: " << dir_path+data[i].file_name << endl;
      return -1;
    }
    int file_size = ((i < file_count -1) ? data[i+1].offset : total_size) - data[i].offset;
    fin.seekg(data[i].offset, ios::beg);
    char* buf = (char *)malloc(file_size);
    fin.read(buf, file_size);
    fout.write(buf,file_size);
    fout.close();
    free(buf);
    filelist << data[i].file_name << endl;
  }

  filelist.close();
  cout << dec << file_count << " files written." << endl;

  return true;
}

bool AKT::unpack_file(char* path) {
  cleanup();

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  bool ret = unpack_file(fin, path);
  fin.close();
  return ret;
}

bool AKT::pack_dir(char* dir_path, char* target_path) {
  if (!fs::exists(dir_path)) {
    cerr << dir_path << " does not exist." << endl;
    return false;
  }

  ifstream fin(fmt::format("{}/{}", dir_path, filelist_name), ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Failed to read file list: " << dir_path << "/" << filelist_name << endl;
    return false;
  }

  ofstream fout(target_path, ios::out|ios::binary);
  if (!fout.is_open()) {
    cerr << "Failed to open file: " << target_path << endl;
    return false;
  }

  AKT_FILE_COUNT_TYPE num_files = 0;
  fin.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
  fout.write(reinterpret_cast<char*>(&num_files), sizeof(num_files));

  if (num_files == 0) {
    fin.close();
    fout.close();
    return true;
  }

  vector<AKTData> meta;
  // ngl this first_offset shit is dumb. It's always 0, and 0-3 is 0x68, 4-7 is 0x88, 8-11 is 0xa8 etc.
  // So start at 0x68 and for every 4, add 32.
  // Makes no sense to me.
  AKT_FILE_OFFSET_TYPE first_offset = 0x68+(num_files/4)*32;
  AKT_FILE_OFFSET_TYPE offset = first_offset;
  char padding_buffer[AKT_PADDING_BUFFER_SIZE];
  memset(padding_buffer, 0, AKT_PADDING_BUFFER_SIZE);

  for (int i=0; i<num_files; i++) {
    AKTData m;
    fin >> m.file_name;
    m.offset = offset;
    fout.write(reinterpret_cast<char*>(&(m.offset)), sizeof(m.offset));
    offset += fs::file_size(fs::path{fmt::format("{}/{}", dir_path, m.file_name)});
    meta.push_back(m);
    //cout << hex << setfill('0') << setw(8) << m.offset << " " << m.file_name << endl;
  }
  fin.close();

  // write extensions.
  for (int i=0; i<num_files; i++) {
    fs::path fpath{meta[i].file_name};
    fs::path ext{fpath.extension()};
    string ext_s = ext.string().substr(1,ext.string().length()-1);
    fout << ext_s;
    fout.write(padding_buffer, 4-(ext_s.length()%4));
  }

  // Weird padding.
  int padding_size=first_offset-fout.tellp();
  while (padding_size > 0) {
    int write_size = padding_size < AKT_PADDING_BUFFER_SIZE ? padding_size : AKT_PADDING_BUFFER_SIZE;
    fout.write(padding_buffer, write_size);
    padding_size -= write_size;
  }

  // Append all the files.
  for (int i=0; i<num_files; i++) {
    fin.open(fs::path{fmt::format("{}/{}", dir_path, meta[i].file_name)}, ios::in|ios::binary);
    fout << fin.rdbuf();
    fin.close();
  }

  fout.close();

  return true;
}

void AKT::cleanup() {

}

int AKT::size() {
  return 0;
}

} // namespace OKAMI_UTILS
