#define RAPIDJSON_HAS_STDSTRING 1

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <fmt/format.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filereadstream.h>
#include <okami-utils/akt.h>
#include "gltf_sca.h"
#include "gltf.h"
using namespace std;
namespace fs = filesystem;
namespace rj = rapidjson;

enum FILE_TYPE {
  AKT_TYPE = 0,
  SCA_TYPE = 1
};

struct GLTFSource {
  fs::path path;
  fs::path bin;
  FILE_TYPE type;
  int material;
};

struct GLTFConfig {
  string name;
  fs::path output_dir;
  vector<GLTFSource> sources;
};

bool read_config(fs::path config, GLTFConfig& conf, OKAMI_UTILS::GLTF& gltf) {
  if (!fs::exists(config)) {
    cerr << config << " does not exist." << endl;
    return false;
  }

  if (!fs::is_regular_file(config)) {
    cerr << config << " is not a regular file." << endl;
    return false;
  }

  int file_size = fs::file_size(config);
  char* config_buffer = (char*)calloc(file_size+1, sizeof(char));
  ifstream fin(config, ios::in);
  if (!fin.is_open()) {
    cerr << "Could not open " << config << endl;
    return false;
  }
  fin.read(config_buffer, file_size);
  fin.close();

  rj::Document cd;
  cd.Parse(config_buffer);

  conf.name = cd["name"].GetString();
  conf.output_dir = fs::path(cd["output"].GetString());
  rj::Value& src = cd["sources"];
  for (int i=0; i<src.Size(); i++) {
    GLTFSource gs;
    gs.path = fs::path(src[i]["path"].GetString());
    gs.bin = fs::path(fmt::format("{}.bin", gs.path.filename().string()));
    gs.type = (FILE_TYPE)src[i]["type"].GetInt();
    gs.material = src[i]["material"].GetInt();
    conf.sources.push_back(gs);
  }
  
  rj::Value& mat = cd["materials"];
  for (int i=0; i<mat.Size(); i++)
    gltf.add_material(mat[i]);

  free(config_buffer);
  config_buffer = nullptr;

  return true;
}

void add_akt(OKAMI_UTILS::GLTF& gltf, GLTFSource src, fs::path out_dir) {
  OKAMI_UTILS::AKT akt;
  akt.parse_file(src.path);

  //OKAMI_UTILS::GLTF gltf;
  int buffer = gltf.add_buffer(akt.get_gltf_buffer_size(), src.bin.string());

  int offset = 0;
  for (int i=0; i<akt.size(); i++) {
    OKAMI_UTILS::AK ak = akt.get(i);
    int size = ak.get_gltf_position_size();
    int p = gltf.add_bufferView(buffer, size, offset);
    gltf.add_accessor_fv3(p, ak.num_coordinates(), ak.get_constraints());
    offset += size;
    size = ak.get_gltf_normal_size();
    int n = gltf.add_bufferView(buffer, size, offset);
    gltf.add_accessor_fv3(n, ak.num_coordinates());
    offset += size;
    size = ak.get_gltf_indices_size();
    int in = gltf.add_bufferView(buffer, size, offset);
    gltf.add_accessor_uss(in, ak.num_indices());
    offset += size;
    if (offset % 4 != 0)
      offset += 4-(offset%4);

    string name = fmt::format("{}.AK.{:#02}", src.path.stem().string(), i);
    int mesh_index = gltf.add_mesh(name, p, n, in, src.material);
    gltf.add_node(mesh_index, name);
  }

  fs::path bin(out_dir);
  bin /= fmt::format("{}.bin", src.path.filename().string());
  ofstream fout(bin, ios::out|ios::binary);
  akt.dump_gltf_binary(fout);
  fout.close();
}

void add_sca(OKAMI_UTILS::GLTF& gltf, GLTFSource src, fs::path out_dir) {
  OKAMI_UTILS::GLTFSCA sca;
  sca.parse_file(src.path);

  if (sca.size() == 0)
    return;

  //OKAMI_UTILS::GLTF gltf;
  int buffer = gltf.add_buffer(sca.get_gltf_buffer_size(), src.bin.string());

  int offset = 0;
  int pos_size = sca.get_gltf_entry_position_size();
  int ind_size = sca.get_gltf_entry_indices_size();

  int in = gltf.add_bufferView(buffer, ind_size, offset);
  gltf.add_accessor_uss(in, sca.num_indices());
  offset += ind_size;
  if (offset % 4 != 0)
    offset += 4-(offset%4);

  for (int i=0; i<sca.size(); i++) {
    int p = gltf.add_bufferView(buffer, pos_size, offset);
    gltf.add_accessor_fv3(p, sca.num_coordinates(), sca.get_constraints(i));
    offset += pos_size;
    // size = ak.get_gltf_normal_size();
    // int n = gltf.add_bufferView(buffer, size, offset);
    // gltf.add_accessor_fv3(n, ak.num_coordinates());
    // offset += size;

    string name = fmt::format("{}.{:02x}", src.path.filename().string(), sca.get(i).effect_index);
    int mesh_index = gltf.add_mesh(name, p, -1, in, src.material);
    gltf.add_node(mesh_index, name);
  }

  fs::path bin(out_dir);
  bin /= fmt::format("{}.bin", src.path.filename().string());
  ofstream fout(bin, ios::out|ios::binary);
  sca.dump_gltf_binary(fout);
  fout.close();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Expect one argument: json config file." << endl;
    return -1;
  }

  fs::path config(argv[1]);
  OKAMI_UTILS::GLTF gltf;
  GLTFConfig conf;
  read_config(config, conf, gltf);

  for (vector<GLTFSource>::iterator it=conf.sources.begin(); it!=conf.sources.end(); it++) {
    switch (it->type) {
      case AKT_TYPE: add_akt(gltf, *it, conf.output_dir); break;
      case SCA_TYPE: add_sca(gltf, *it, conf.output_dir); break;
      default: cerr << "Unknown source type: " << it->type << endl; continue;
    }
  }

  fs::path out(conf.output_dir);
  out /= conf.name;
  out += ".gltf";
  ofstream fout(out, ios::out|ios::binary);
  gltf.write(fout);
  fout.close();

  return 0;
}