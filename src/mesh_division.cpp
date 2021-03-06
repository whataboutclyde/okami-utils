#include <iomanip>
#include <iostream>
#include <okami-utils/mesh_division.h>
#include <glm/gtc/packing.hpp>
using namespace std;

namespace OKAMI_UTILS {

bool MeshDivision::process_file(ifstream& fin) {
  fpos_t md_start = fin.tellg();
  fin.read(reinterpret_cast<char*>(&header), sizeof(header));

  if (header.v_offset != 0) {
    fin.seekg(md_start+header.v_offset, ios::beg);
    for (int i=0; i<header.vertices; i++) {
      SCRVertex sv;
      fin.read(reinterpret_cast<char*>(&sv), sizeof(sv));
      vertices.push_back(sv);
    }
  }

  if (header.unknown1 != 0) {
    cerr << "unknown1 isn't 0: " << hex << setfill('0') << setw(8) << header.unknown1 << endl;
    return false;
  }

  if (header.itm_offset != 0) {
    fin.seekg(md_start+header.itm_offset, ios::beg);
    for (int i=0; i<header.vertices; i++) {
      PackedPair<uint16_t> ie;
      fin.read(reinterpret_cast<char*>(&ie), sizeof(ie));
      itm.push_back(ie);
    }
  }

  if (header.tcw_offset != 0) {
    fin.seekg(md_start+header.tcw_offset, ios::beg);
    for (int i=0; i<header.vertices; i++) {
      int32_t te;
      fin.read(reinterpret_cast<char*>(&te), sizeof(te));
      tcw.push_back(te);
    }
  }

  if (header.tuv_offset != 0) {
    fin.seekg(md_start+header.tuv_offset, ios::beg);
    for (int i=0; i<header.vertices; i++) {
      PackedPair<uint16_t> te;
      fin.read(reinterpret_cast<char*>(&te), sizeof(te));
      tuv.push_back(te);
    }
  }

  build_indices();

  return true;
}

int MeshDivision::size() {
  return vertices.size();
}

int MeshDivision::get_gltf_buffer_size() {
  int size = 0;
  size += get_gltf_indices_size();
  int pad=size%4;
  if (pad != 0)
    size += 4-pad;
  size += get_gltf_vertices_size();
  pad=size%4;
  if (pad != 0)
    size += 4-pad;
  size += get_gltf_itm_size();
  pad=size%4;
  if (pad != 0)
    size += 4-pad;
  size += get_gltf_tcw_size();
  pad=size%4;
  if (pad != 0)
    size += 4-pad;
  size += get_gltf_tuv_size();
  pad=size%4;
  if (pad != 0)
    size += 4-pad;
  return size;
}

int MeshDivision::num_indices() {
  return indices.size();
}

int MeshDivision::get_gltf_indices_size() {
  return indices.size()*sizeof(uint16_t);
}

int MeshDivision::num_coordinates() {
  return vertices.size();
}

int MeshDivision::get_gltf_vertices_size() {
  return vertices.size()*sizeof(float)*3;
}

int MeshDivision::get_gltf_itm_size() {
  return itm.size()*sizeof(PackedPair<uint16_t>);
}

int MeshDivision::get_gltf_tcw_size() {
  return tcw.size()*sizeof(uint32_t);
}

int MeshDivision::get_gltf_tuv_size() {
  return tuv.size()*sizeof(PackedPair<uint16_t>);
}

void MeshDivision::build_indices() {
  // Too few to make any triangles...
  if (header.vertices < 3)
    return;

  for (uint16_t i=2; i<header.vertices; i++) {
    // If this is a no tri strip marker and so was the last, skip forward.
    if (/*vertices[i].conn == NOT_TRI_STRIP_CONN &&*/ vertices[i].conn == NOT_TRI_STRIP_CONN)
      continue;

    indices.push_back(i-2);
    indices.push_back(i-1);
    indices.push_back(i);
  }
  
  cout << indices.size() << endl;
}

void MeshDivision::dump_gltf_binary(ofstream& fout) {
  for (vector<uint16_t>::iterator it=indices.begin(); it!=indices.end(); it++)
    fout.write(reinterpret_cast<char*>(&*it),sizeof(*it));
  int pad = fout.tellp()%4;
  if (pad!=0) {
    pad = 4-pad;
    while (pad-- > 0)
      fout.put('\0');
  }

  for (int i=0; i<vertices.size(); i++) {
    PackedTuple<float> ft;
    ft.x = vertices[i].coordinates.x;
    ft.y = vertices[i].coordinates.y;
    ft.z = vertices[i].coordinates.z;
    fout.write(reinterpret_cast<char*>(&ft),sizeof(ft));
  }

  pad = fout.tellp()%4;
  if (pad!=0) {
    pad = 4-pad;
    while (pad-- > 0)
      fout.put('\0');
  }

  for (vector<PackedPair<uint16_t>>::iterator it=itm.begin(); it!=itm.end(); it++)
    fout.write(reinterpret_cast<char*>(&it->x),sizeof(it->x));

  pad = fout.tellp()%4;
  if (pad!=0) {
    pad = 4-pad;
    while (pad-- > 0)
      fout.put('\0');
  }

  for (vector<uint32_t>::iterator it=tcw.begin(); it!=tcw.end(); it++)
    fout.write(reinterpret_cast<char*>(&*it),sizeof(*it));

  pad = fout.tellp()%4;
  if (pad!=0) {
    pad = 4-pad;
    while (pad-- > 0)
      fout.put('\0');
  }

  for (vector<PackedPair<uint16_t>>::iterator it=tuv.begin(); it!=tuv.end(); it++)
    fout.write(reinterpret_cast<char*>(&*it),sizeof(*it));

  pad = fout.tellp()%4;
  if (pad!=0) {
    pad = 4-pad;
    while (pad-- > 0)
      fout.put('\0');
  }
}

}