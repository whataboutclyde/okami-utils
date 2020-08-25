#include <iostream>
#include <fstream>
#include <string>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include "okami-utils/scr.h"
using namespace std;

int main(int argc, char *argv[]) {  
  if (argc == 1) {
    cerr << "Didn't supply .SCR or .MD file.";
    return -1;
  }

  char* path = argv[1];
  string path_string(path);

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  SCRHeader header;
  fin.read(reinterpret_cast<char*>(&header),sizeof(header));

  //cout << path << endl;
  if (header.scr_id != SCR_ID_STRING)
    cerr << "wtf scr_id isn't scr?? " << hex << header.scr_id << endl;

  //cout << "num_mdb: " << hex << header.num_mdb << endl;
  if (header.padding != 0) {
    cerr << "guess it isn't always padding..." << hex << header.padding << endl;
  }

  uint32_t* submesh_ids = (uint32_t*)calloc(header.num_mdb,sizeof(uint32_t));
  fin.read(reinterpret_cast<char*>(submesh_ids), header.num_mdb*sizeof(uint32_t));

  
  
  fin.close();
}