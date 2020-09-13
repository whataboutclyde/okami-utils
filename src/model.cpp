#include <okami-utils/model.h>
#include <okami-utils/submesh.h>
#include <iostream>
using namespace std;

namespace OKAMI_UTILS {

bool Model::process_file(ifstream& fin) {
  OKAMI_UTILS::SCRHeader header;
  fin.read(reinterpret_cast<char*>(&header),sizeof(header));

  if (header.scr_id != SCR_ID_STRING) {
    cerr << "Incorrect file header: " << hex << setfill('0') << setw(8) << header.scr_id << endl;
    return false;
  }

  uint32_t* submesh_end_offsets = (uint32_t*)calloc(header.end_entry_count,sizeof(uint32_t));
  fin.read(reinterpret_cast<char*>(submesh_end_offsets), header.end_entry_count*sizeof(uint32_t));
  align(fin,16);
  
  // Read dumb submeshes here.
  while (fin.tellg() < submesh_end_offsets[0]) {
    Submesh sm;
    sm.process_file(fin);
    submeshes.push_back(sm);
    align(fin,16);
  }

  for (int i=0; i<header.end_entry_count; i++) {
    SCREndStuff end;
    fin.seekg(submesh_end_offsets[i], ios::beg);
    fin.read(reinterpret_cast<char*>(&end), sizeof(end));
    end_stuff.push_back(end);
  }
  free(submesh_end_offsets);
  return true;
}

int Model::size() {
  return submeshes.size();
}

int Model::get_gltf_buffer_size() {
  int size = 0;
  for (int i=0; i<submeshes.size(); i++)
    size += submeshes[i].get_gltf_buffer_size();
  return size;
}

void Model::dump_gltf_binary(ofstream& fout) {
  for (int i=0; i<submeshes.size(); i++)
    submeshes[i].dump_gltf_binary(fout);
}

} // namespace OKAMI_UTILS