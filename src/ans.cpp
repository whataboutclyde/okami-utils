#include <iostream>
#include <iterator>
#include <okami-utils/ans.h>
using namespace std;

namespace OKAMI_UTILS {

ANS::ANS(char* path) {
  parse_file(path);
}

ANS::~ANS() {
  cleanup();
}

bool ANS::parse_file(ifstream& fin) {
  ANS_ENTRY_COUNT_TYPE entry_count = 0;
  fin.seekg(0, ios::beg);
  fin.read(reinterpret_cast<char *>(&entry_count), sizeof(entry_count));

  for (int i=0;i<entry_count;i++) {
    ANSEntry entry;
    fin.read(reinterpret_cast<char *>(&entry), ANS_ENTRY_SIZE);
    groups.insert(pair<uint8_t, ANSEntry>(entry.group_id, entry));
    group_ids.push_back(entry.group_id);
    //entries.push_back(entry);
    //reading in padding for now
    //fin.seekg(ANS_ENTRY_PADDING_SIZE, ios::cur);
  }
  group_ids.sort();
  group_ids.unique();
  return true;
}

bool ANS::parse_file(char* path) {
  cleanup();

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  bool ret = parse_file(fin);
  fin.close();
  return ret;
}

void ANS::write_file(ofstream& fout) {
  // int file_size = sizeof(ANS_ENTRY_COUNT_TYPE) + (ANS_ENTRY_SIZE + ANS_ENTRY_PADDING_SIZE)*entries.size();
  // int padding = 32-(file_size % 32); // padding for 32 byte alignment.
  // char* pad_buffer = (char*)calloc(sizeof(uint8_t),padding > ANS_ENTRY_PADDING_SIZE ? padding : ANS_ENTRY_PADDING_SIZE);

  // ANS_ENTRY_COUNT_TYPE entry_count = entries.size();
  // fout.write(reinterpret_cast<char *>(&entry_count), sizeof(ANS_ENTRY_COUNT_TYPE));
  // for (int i=0;i<entry_count;i++) {
  //   fout.write(reinterpret_cast<char *>(&entries[i]), ANS_ENTRY_SIZE);
  //   //reading in padding for now
  //   //fout.write(pad_buffer, ANS_ENTRY_PADDING_SIZE);
  // }
  // fout.write(pad_buffer, padding);
  // free(pad_buffer);
}

bool ANS::write_file(char* path) {
  // ofstream fout(path, ios::out|ios::binary);
  // if (!fout.is_open()) {
  //   cerr << "Couldn't open file " << path << endl;
  //   return false;
  // }
  // write_file(fout);
  // fout.close();
  // return true;
  return false;
}

void ANS::cleanup() {
  groups.clear();
}

int ANS::size() {
  return group_ids.size();
}

list<uint8_t> ANS::get_keys() {
  return group_ids;
}

vector<ANSEntry> ANS::get(uint8_t i) {
  vector<ANSEntry> entries;
  std::pair<multimap<uint8_t, ANSEntry>::iterator, multimap<uint8_t, ANSEntry>::iterator> result = groups.equal_range(i);
  for (multimap<uint8_t, ANSEntry>::iterator it=result.first; it!=result.second; it++)
    entries.push_back(it->second);
  return entries;
}

} // namespace OKAMI_UTILS
