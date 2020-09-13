#include <iomanip>
#include <iostream>
#include <okami-utils/submesh.h>
#include <okami-utils/mesh_division.h>
using namespace std;

namespace OKAMI_UTILS {

bool Submesh::process_file(ifstream& fin) {
  fpos_t mdb_start = fin.tellg();
  fin.read(reinterpret_cast<char*>(&header), sizeof(header));
  if (header.mdb_id != MDB_ID_STRING) {
    cerr << "Submesh doesn't have header mdb: " << hex << setfill('0') << setw(8) << header.mdb_id << endl;
    return false;
  }

  uint32_t* mdiv_offsets = (uint32_t*)calloc(header.mesh_divisions, sizeof(uint32_t));
  fin.read(reinterpret_cast<char*>(mdiv_offsets), header.mesh_divisions*sizeof(uint32_t));
  
  for (int i=0; i<header.mesh_divisions; i++) {
    MeshDivision md;
    fin.seekg(mdb_start+mdiv_offsets[i], ios::beg);
    md.process_file(fin);
    divisions.push_back(md);
  }

  free(mdiv_offsets);
  return true;
}

int Submesh::size() {
  return divisions.size();
}

int Submesh::get_gltf_buffer_size() {
  int size = 0;
  for (int i=0; i<divisions.size(); i++)
    size+=divisions[i].get_gltf_buffer_size();
  return size;
}

void Submesh::dump_gltf_binary(ofstream& fout) {
  for (int i=0; i<divisions.size(); i++)
    divisions[i].dump_gltf_binary(fout);
}

} //namespace OKAMI_UTILS