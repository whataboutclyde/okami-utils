#define RAPIDJSON_HAS_STDSTRING 1

#include <iostream>
#include <filesystem>
#include <fmt/format.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <okami-utils/akt.h>
using namespace std;
namespace fs = filesystem;
namespace rj = rapidjson;

#define OUT_FILE "F:\\okami-utils\\test.gltf"
#define LEVEL_FILE "F:\\okami\\st1\\r122.dat_dir\\r122.AKT"

void write_asset(rj::Document& gltf, rj::Document::AllocatorType& allocator) {
  rj::Value version;
  version.SetObject();
  version.AddMember("version", "2.0", allocator);
  gltf.AddMember("asset", version, allocator);
}

enum MATERIAL_ID {
  COLLISION_MATERIAL = 0,
  EXIT_MATERIAL,
  DIALOGUE_MATERIAL
};

// Need to add them in the same order as the enum.
void write_materials(rj::Document& gltf, rj::Document::AllocatorType& allocator) {
  rj::Value emissiveFactor(rj::kArrayType);
  emissiveFactor.PushBack(0, allocator).PushBack(0, allocator).PushBack(0, allocator);

  rj::Value materials(rj::kArrayType);

  // Collision material
  rj::Value akt_material(rj::kObjectType);
  akt_material.AddMember("doubleSided", true, allocator);
  akt_material.AddMember("emissiveFactor", emissiveFactor, allocator);
  akt_material.AddMember("name", "Collision", allocator);
  rj::Value pbtMetallicRoughness(rj::kObjectType);
  rj::Value baseColorFactor(rj::kArrayType);
  baseColorFactor.PushBack(0.800000011920929, allocator); // R
  baseColorFactor.PushBack(0.800000011920929, allocator); // G
  baseColorFactor.PushBack(0.800000011920929, allocator); // B
  baseColorFactor.PushBack(1, allocator); // alpha
  pbtMetallicRoughness.AddMember("baseColorFactor", baseColorFactor, allocator);
  pbtMetallicRoughness.AddMember("metallicFactor", 0, allocator);
  pbtMetallicRoughness.AddMember("roughnessFactor", 0.5, allocator);
  akt_material.AddMember("pbtMetallicRoughness", pbtMetallicRoughness, allocator);
  materials.PushBack(akt_material, allocator);

  // Exit material

  gltf.AddMember("materials", materials, allocator);
}

int main(int argc, char* argv[]) {
  fs::path path(OUT_FILE);

  rj::Document gltf;
  gltf.SetObject();
  rj::Document::AllocatorType& allocator = gltf.GetAllocator();
  int num_nodes = 0;

  write_asset(gltf, allocator);
  // Just always going to be single scene.
  gltf.AddMember("scene", 0, allocator);

  fs::path level_file(LEVEL_FILE);
  OKAMI_UTILS::AKT level;
  level.parse_file(level_file);
  num_nodes += level.size();

  rj::Value nodes(rj::kArrayType);
  rj::Value scenes(rj::kArrayType);
  rj::Value scene_nodes(rj::kArrayType);
  rj::Value meshes(rj::kArrayType);
  rj::Value accessors(rj::kArrayType);
  rj::Value buffer_views(rj::kArrayType);
  unsigned int offset = 0;
  cout << level.size() << endl;
  for (int i=0; i<level.size(); i++) {
    rj::Value ak_node(rj::kObjectType);
    ak_node.AddMember("mesh", i, allocator);
    string name = fmt::format("{}.{:#02}.AK", level_file.stem().string(), i);
    ak_node.AddMember("name", name, allocator);
    nodes.PushBack(ak_node, allocator);
    scene_nodes.PushBack(i, allocator);
    rj::Value ak_mesh_node(rj::kObjectType);
    ak_mesh_node.AddMember("name", name, allocator);
    rj::Value primitives(rj::kArrayType);
    rj::Value prim_entry(rj::kObjectType);
    rj::Value attributes(rj::kObjectType);
    attributes.AddMember("POSITION", 3*i, allocator);
    attributes.AddMember("NORMAL", 3*i+1, allocator);
    prim_entry.AddMember("attributes", attributes, allocator);
    prim_entry.AddMember("indices", 3*i+2, allocator);
    prim_entry.AddMember("material", COLLISION_MATERIAL, allocator);
    primitives.PushBack(prim_entry, allocator);
    ak_mesh_node.AddMember("primitives", primitives, allocator);
    meshes.PushBack(ak_mesh_node, allocator);

    OKAMI_UTILS::AK ak = level.get(i);

    rj::Value coord_access(rj::kObjectType);
    coord_access.AddMember("bufferView", 3*i, allocator);
    coord_access.AddMember("componentType", 5122, allocator);
    coord_access.AddMember("count", ak.num_coordinates(), allocator);
    rj::Value max(rj::kArrayType);
    max.PushBack((int16_t)ak.header.max_x, allocator).PushBack((int16_t)ak.header.max_y, allocator).PushBack((int16_t)ak.header.max_z, allocator);
    coord_access.AddMember("max", max, allocator);
    rj::Value min(rj::kArrayType);
    min.PushBack((int16_t)ak.header.min_x, allocator).PushBack((int16_t)ak.header.min_y, allocator).PushBack((int16_t)ak.header.min_z, allocator);
    coord_access.AddMember("min", min, allocator);
    coord_access.AddMember("type", "VEC3", allocator);
    accessors.PushBack(coord_access, allocator);

    rj::Value coord_buff(rj::kObjectType);
    coord_buff.AddMember("buffer", 0, allocator);
    coord_buff.AddMember("byteLength", sizeof(OKAMI_UTILS::Int16Tuple)*ak.num_coordinates(), allocator);
    coord_buff.AddMember("byteOffset", offset, allocator);
    offset += sizeof(OKAMI_UTILS::Int16Tuple)*ak.num_coordinates();
    buffer_views.PushBack(coord_buff, allocator);

    rj::Value vn_access(rj::kObjectType);
    vn_access.AddMember("bufferView", 3*i+1, allocator);
    vn_access.AddMember("componentType", 5120, allocator);
    vn_access.AddMember("count", ak.num_coordinates(), allocator);
    vn_access.AddMember("type", "VEC3", allocator);
    accessors.PushBack(vn_access, allocator);

    rj::Value vn_buff(rj::kObjectType);
    vn_buff.AddMember("buffer", 0, allocator);
    vn_buff.AddMember("byteLength", sizeof(OKAMI_UTILS::Int8Tuple)*ak.num_coordinates(), allocator);
    vn_buff.AddMember("byteOffset", offset, allocator);
    offset += sizeof(OKAMI_UTILS::Int8Tuple)*ak.num_coordinates();
    buffer_views.PushBack(vn_buff, allocator);

    rj::Value index_access(rj::kObjectType);
    index_access.AddMember("bufferView", 3*i+2, allocator);
    index_access.AddMember("componentType", 5123, allocator);
    index_access.AddMember("count", ak.num_index_sets()*3, allocator);
    index_access.AddMember("type", "SCALAR", allocator);
    accessors.PushBack(index_access, allocator);

    rj::Value index_buff(rj::kObjectType);
    index_buff.AddMember("buffer", 0, allocator);
    index_buff.AddMember("byteLength", sizeof(uint16_t)*ak.num_index_sets()*3, allocator);
    index_buff.AddMember("byteOffset", offset, allocator);
    offset += sizeof(uint16_t)*ak.num_index_sets()*3;
    offset += offset%2;
    buffer_views.PushBack(index_buff, allocator);
  }
  rj::Value scene_val(rj::kObjectType);
  scene_val.AddMember("name", "scene", allocator);
  scene_val.AddMember("nodes", scene_nodes, allocator);
  scenes.PushBack(scene_val, allocator);
  gltf.AddMember("scenes", scenes, allocator);
  gltf.AddMember("nodes", nodes, allocator);

  write_materials(gltf, allocator);

  gltf.AddMember("meshes", meshes, allocator);
  gltf.AddMember("accessors", accessors, allocator);
  gltf.AddMember("bufferViews", buffer_views, allocator);

  rj::Value buffers(rj::kArrayType);
  rj::Value buffers_entry(rj::kObjectType);
  buffers_entry.AddMember("byteLength", offset, allocator);
  buffers.PushBack(buffers_entry, allocator);
  gltf.AddMember("buffers", buffers, allocator);

  rj::StringBuffer json;
  rj::Writer<rj::StringBuffer> writer(json);
  gltf.Accept(writer);

  // Header is:
  //   glTF
  //   02 00 00 00 (version)
  //   entire_file_size
  //   json_length
  //   JSON
  ofstream fout(OUT_FILE, ios::out|ios::binary);
  fout << "glTF";
  int version = 2;
  fout.write(reinterpret_cast<char*>(&version), sizeof(int));
  int json_len = json.GetSize();
  int file_size = 20 + offset + json_len + 8;
  fout.write(reinterpret_cast<char*>(&file_size), sizeof(unsigned int));
  fout.write(reinterpret_cast<char*>(&json_len), sizeof(int));
  fout << "JSON";
  // Dump the json here.
  fout << json.GetString();
  // Before binary data header:
  //   binary_data_size
  //   BIN\0
  fout.write(reinterpret_cast<char*>(&offset), sizeof(unsigned int));
  fout << "BIN";
  fout.put('\0');
  // Write the binary data.
  for (int i=0; i<level.size(); i++)
    level.get(i).dump_binary(fout);
  fout.close();

  return 0;
}