#include "gltf.h"
using namespace rapidjson;

namespace OKAMI_UTILS {

const int GLTF::gltf_version = 2;

enum {
  SCENES_INDEX = 0,
  NODES_INDEX,
  MATERIALS_INDEX,
  MESHES_INDEX,
  ACCESSORS_INDEX,
  BUFFERVIEWS_INDEX,
  BUFFERS_INDEX,
  NUM_DOC_ARRAYS
};

// Make sure this is the same order as the enum.
const char* GLTF::array_names[] = {
  "scenes",
  "nodes",
  "materials",
  "meshes",
  "accessors",
  "bufferViews",
  "buffers"
};

GLTF::GLTF() {
  doc.SetObject();

  Value asset(kObjectType);
  asset.AddMember("version", "2.0", al);
  doc.AddMember("asset", asset, al);

  // Only one scene.
  doc.AddMember("scene", 0, al);

  for (int i=0; i<NUM_DOC_ARRAYS; i++) {
    Value v(kArrayType);
    doc.AddMember(StringRef(array_names[i]), v, al);
  }

  // Set up scenes because only one.
  Value scene(kObjectType);
  scene.AddMember("name", "scene", al);
  Value scene_nodes(kArrayType);
  scene.AddMember("nodes", scene_nodes, al);

  Value& scenes = doc[array_names[SCENES_INDEX]];
  scenes.PushBack(scene, al);
}

int GLTF::add_buffer(int len, std::string uri="") {
  if (len < 1)
    return -1;
  Value b(kObjectType);
  b.AddMember("byteLength", len, al);
  if (uri.length() > 0)
    b.AddMember("uri", uri, al);
  Value& buffers = doc[array_names[BUFFERS_INDEX]];
  buffers.PushBack(b, al);
  return buffers.Size()-1;
}

int GLTF::add_bufferView(int buffer, int len, int offset) {
  if (buffer < 0 || len < 1 || offset < 0)
    return -1;

  Value bv(kObjectType);
  bv.AddMember("buffer", buffer, al);
  bv.AddMember("byteLength", len, al);
  bv.AddMember("byteOffset", offset, al);
  Value &bufferViews = doc[array_names[BUFFERVIEWS_INDEX]];
  bufferViews.PushBack(bv, al);
  return bufferViews.Size()-1;
}

int GLTF::add_mesh(std::string name, int pos, int norm, int ind, int mat) {
  Value m(kObjectType);
  m.AddMember("name", name, al);

  Value attr(kObjectType);
  attr.AddMember("POSITION", pos, al);
  if (norm != -1)
    attr.AddMember("NORMAL", norm, al);
  Value prim(kObjectType);
  prim.AddMember("attributes", attr, al);
  prim.AddMember("indices", ind, al);
  prim.AddMember("material", mat, al);
  Value p(kArrayType);
  p.PushBack(prim, al);
  m.AddMember("primitives", p, al);

  Value &meshes = doc[array_names[MESHES_INDEX]];
  meshes.PushBack(m, al);
  return meshes.Size()-1;
}

void GLTF::add_node(int mesh, std::string name) {
  // Add to list of nodes
  Value n(kObjectType);
  n.AddMember("mesh", mesh, al);
  n.AddMember("name", name, al);
  Value &nodes = doc[array_names[NODES_INDEX]];
  nodes.PushBack(n, al);

  // Only one scene so adding to that scene's list of nodes.
  Value& scene_nodes = doc[array_names[SCENES_INDEX]][0]["nodes"];
  scene_nodes.PushBack(mesh, al);
}

void GLTF::add_accessor_fv3(int view, int count) {
  Value a(kObjectType);
  a.AddMember("bufferView", view, al);
  a.AddMember("componentType", 5126, al);
  a.AddMember("count", count, al);
  a.AddMember("type", "VEC3", al);

  Value &accessors = doc[array_names[ACCESSORS_INDEX]];
  accessors.PushBack(a, al);
}

void GLTF::add_accessor_fv3(int view, int count, FloatConstraints constraints) {
  Value a(kObjectType);
  a.AddMember("bufferView", view, al);
  a.AddMember("componentType", 5126, al);
  a.AddMember("count", count, al);

  Value max(kArrayType);
  max.PushBack(constraints.max_x, al);
  max.PushBack(constraints.max_y, al);
  max.PushBack(constraints.max_z, al);
  a.AddMember("max", max, al);

  Value min(kArrayType);
  min.PushBack(constraints.min_x, al);
  min.PushBack(constraints.min_y, al);
  min.PushBack(constraints.min_z, al);
  a.AddMember("min", min, al);

  a.AddMember("type", "VEC3", al);

  Value &accessors = doc[array_names[ACCESSORS_INDEX]];
  accessors.PushBack(a, al);
}

void GLTF::add_accessor_uss(int view, int count) {
  Value a(kObjectType);
  a.AddMember("bufferView", view, al);
  a.AddMember("componentType", 5123, al);
  a.AddMember("count", count, al);
  a.AddMember("type", "SCALAR", al);

  Value &accessors = doc[array_names[ACCESSORS_INDEX]];
  accessors.PushBack(a, al);
}

void GLTF::add_material(const Value& mat) {
  Value m(mat, al);
  Value& materials = doc[array_names[MATERIALS_INDEX]];
  materials.PushBack(m, al);
}

void GLTF::write(ofstream& fout) {
  StringBuffer json;
  Writer<StringBuffer> writer(json);
  doc.Accept(writer);

  fout << "glTF";
  fout.write(reinterpret_cast<const char*>(&gltf_version), sizeof(gltf_version));
  int file_size = json.GetSize() + 20;
  int pad = file_size % 4;
  if (pad != 0)
    pad = 4-pad;
  file_size += pad;
  fout.write(reinterpret_cast<char*>(&file_size), sizeof(file_size));
  file_size -= 20;
  fout.write(reinterpret_cast<char*>(&file_size), sizeof(file_size));
  fout << "JSON";
  fout << json.GetString();
  for (int i=0; i<pad; i++)
    fout.put(' ');
}

} // namespace OKAMI_UTILS