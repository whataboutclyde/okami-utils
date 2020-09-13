#include "gltf.h"
using namespace rapidjson;

namespace OKAMI_UTILS {

const int GLTF::gltf_version = 2;

enum {
  SCENES_INDEX = 0,
  NODES_INDEX,
  MATERIALS_INDEX,
  IMAGES_INDEX,
  SAMPLERS_INDEX,
  TEXTURES_INDEX,
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
  "images",
  "samplers",
  "textures",
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

  // Set up one sampler unless I find out I need others...
  Value sampler(kObjectType);
  sampler.AddMember("magFilter", 9729, al);
  sampler.AddMember("minFilter", 9987, al);
  sampler.AddMember("wrapS", 10497, al);
  sampler.AddMember("wrapT", 10497, al);
  Value& samplers = doc[array_names[SAMPLERS_INDEX]];
  samplers.PushBack(sampler, al);
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

void GLTF::change_buffer_length(int buffer, int len) {
  Value& b = doc[array_names[BUFFERS_INDEX]][buffer]["byteLength"];
  b=len;
}

int GLTF::add_bufferView(int buffer, int len, int& offset) {
  if (buffer < 0 || len < 1 || offset < 0)
    return -1;

  Value bv(kObjectType);
  bv.AddMember("buffer", buffer, al);
  bv.AddMember("byteLength", len, al);
  bv.AddMember("byteOffset", offset, al);
  Value &bufferViews = doc[array_names[BUFFERVIEWS_INDEX]];
  bufferViews.PushBack(bv, al);
  offset += len;
  return bufferViews.Size()-1;
}

int GLTF::add_mesh(std::string name, int pos, int norm, int ind, int itm, int tcw, int mat, int tex) {
  Value m(kObjectType);
  m.AddMember("name", name, al);

  Value attr(kObjectType);
  attr.AddMember("POSITION", pos, al);
  if (norm != -1)
    attr.AddMember("NORMAL", norm, al);
  if (itm != -1)
    attr.AddMember("TEXCOORD_0", itm, al);
  if (tcw != -1)
    attr.AddMember("WEIGHTS_0", tcw, al);
  Value prim(kObjectType);
  prim.AddMember("attributes", attr, al);
  prim.AddMember("indices", ind, al);
  if (mat != -1)
    prim.AddMember("material", mat, al);
  if (tex != -1) {
    Value t(kObjectType);
    t.AddMember("index", tex, al);   
    prim.AddMember("textureInfo", t, al);
  }
  Value p(kArrayType);
  p.PushBack(prim, al);
  m.AddMember("primitives", p, al);

  Value &meshes = doc[array_names[MESHES_INDEX]];
  meshes.PushBack(m, al);
  return meshes.Size()-1;
}

int GLTF::add_parent_node(std::string name, int parent_node) {
  Value n(kObjectType);
  n.AddMember("name", name, al);
  Value c(kArrayType);
  n.AddMember("children", c, al);
  Value &nodes = doc[array_names[NODES_INDEX]];
  nodes.PushBack(n, al);

  // Only one scene so adding to that scene's list of nodes.
  Value& scene_nodes = doc[array_names[SCENES_INDEX]][0]["nodes"];
  scene_nodes.PushBack(nodes.Size()-1, al);

  if (parent_node >=0) {
    Value& pn = doc[array_names[NODES_INDEX]][parent_node]["children"];
    pn.PushBack(nodes.Size()-1, al);
  }

  return nodes.Size()-1;
}

void GLTF::add_node(int mesh, std::string name, int parent_node) {
  // Add to list of nodes
  Value n(kObjectType);
  n.AddMember("mesh", mesh, al);
  n.AddMember("name", name, al);
  Value &nodes = doc[array_names[NODES_INDEX]];
  nodes.PushBack(n, al);

  // Only one scene so adding to that scene's list of nodes.
  Value& scene_nodes = doc[array_names[SCENES_INDEX]][0]["nodes"];
  scene_nodes.PushBack(nodes.Size()-1, al);

  if (parent_node >=0) {
    Value& pn = doc[array_names[NODES_INDEX]][parent_node]["children"];
    pn.PushBack(nodes.Size()-1, al);
  }
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

void GLTF::add_accessor_ubv4(int view, int count) {
  Value a(kObjectType);
  a.AddMember("bufferView", view, al);
  a.AddMember("componentType", 5121, al);
  a.AddMember("count", count, al);
  a.AddMember("type", "VEC4", al);

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

int GLTF::add_image(string path) {
  Value i(kObjectType);
  i.AddMember("uri", path, al);

  Value &images = doc[array_names[IMAGES_INDEX]];
  images.PushBack(i, al);

  return images.Size()-1;
}

void GLTF::add_texture(int src) {
  Value t(kObjectType);
  t.AddMember("sampler", 0, al);
  t.AddMember("source", src, al);

  Value &textures = doc[array_names[TEXTURES_INDEX]];
  textures.PushBack(t, al);
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