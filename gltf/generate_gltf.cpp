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
#include "wrappers/gltf_akt.h"
#include "wrappers/gltf_zone.h"
#include "gltf.h"
using namespace std;
namespace fs = filesystem;
namespace rj = rapidjson;

enum FILE_TYPE {
  GLTFAKT_TYPE = 0,
  ZONE_TYPE = 1
};

enum MATERIAL_TYPE {
  MATERIAL_COLLISION = 0,
  MATERIAL_EXIT,
  MATERIAL_BIT_FLAG,
  MATERIAL_EXAMINE,
  MATERIAL_ISSUN_INDICATOR,
  MATERIAL_UNKNOWN
};

struct GLTFLevel {
  fs::path input_dir;
  string id;
  string name;
};

struct GLTFSource {
  string archive;
  string extension;
  FILE_TYPE type;
};

struct GLTFConfig {
  fs::path game_files;
  fs::path output_dir;
  vector<GLTFSource> sources;
  vector<GLTFLevel> levels;
  rj::Document materials;
};

bool read_config(fs::path config, GLTFConfig& conf) {
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

  conf.game_files = fs::path(cd["gameFiles"].GetString());
  conf.output_dir = fs::path(cd["output"].GetString());

  rj::Value& src = cd["sources"];
  for (int j=0; j<src.Size(); j++) {
    string archive = src[j]["archive"].GetString();
    rj::Value& sub = src[j]["files"];
    for (int i=0; i<sub.Size(); i++) {
      GLTFSource gs;
      gs.archive = archive;
      gs.extension = sub[i]["extension"].GetString();
      gs.type = (FILE_TYPE)sub[i]["type"].GetInt();
      conf.sources.push_back(gs);
    }
  }

  rj::Value& levels = cd["levels"];
  for (int j=0; j<levels.Size(); j++) {
    fs::path region = conf.game_files;
    region /= levels[j]["region"].GetString();
    rj::Value& maps = levels[j]["maps"];
    for (int i=0; i<maps.Size(); i++) {
      GLTFLevel lvl;
      lvl.id = maps[i]["id"].GetString();
      lvl.input_dir = region;
      lvl.name = maps[i]["name"].GetString();
      conf.levels.push_back(lvl);
    }
  }
  
  rj::Value& mat = cd["materials"];
  rj::Value materials(kArrayType);
  rj::Document::AllocatorType& al = conf.materials.GetAllocator();
  for (int i=0; i<mat.Size(); i++) {
    Value m(mat[i], al);
    materials.PushBack(m, al);
  }
  conf.materials.SetObject();
  conf.materials.AddMember("materials", materials, al);

  free(config_buffer);
  config_buffer = nullptr;

  return true;
}

void add_akt(OKAMI_UTILS::GLTF& gltf, fs::path path, fs::path out_dir) {
  OKAMI_UTILS::GLTFAKT akt;
  akt.parse_file(path);

  fs::path bin_short(path.filename());
  bin_short += ".bin";
  int buffer = gltf.add_buffer(akt.get_gltf_buffer_size(), bin_short.string());

  int offset = 0;
  for (int i=0; i<akt.size(); i++) {
    OKAMI_UTILS::AK ak = akt.get(i);
    int size = ak.get_gltf_position_size();
    int p = gltf.add_bufferView(buffer, size, offset);
    gltf.add_accessor_fv3(p, ak.num_coordinates(), ak.get_constraints());
    size = ak.get_gltf_normal_size();
    int n = gltf.add_bufferView(buffer, size, offset);
    gltf.add_accessor_fv3(n, ak.num_coordinates());
    size = ak.get_gltf_indices_size();
    int in = gltf.add_bufferView(buffer, size, offset);
    gltf.add_accessor_uss(in, ak.num_indices());
    if (offset % 4 != 0)
      offset += 4-(offset%4);

    string name = fmt::format("{}.AK.{:#02}", path.stem().string(), i);
    int mesh_index = gltf.add_mesh(name, p, n, in, MATERIAL_COLLISION);
    gltf.add_node(mesh_index, name);
  }

  fs::path bin(out_dir);
  bin /= bin_short;
  ofstream fout(bin, ios::out|ios::binary);
  akt.dump_gltf_binary(fout);
  fout.close();
}

void add_zone(OKAMI_UTILS::GLTF& gltf, fs::path path, fs::path out_dir) {
  OKAMI_UTILS::GLTFZone zone;
  zone.parse_file(path);

  if (zone.size() == 0)
    return;

  fs::path bin_short(path.filename());
  bin_short += ".bin";
  int buffer = gltf.add_buffer(zone.get_gltf_buffer_size(), bin_short.string());

  int offset = 0;
  int ind_size = zone.get_gltf_quad_indices_size();
  int qin = gltf.add_bufferView(buffer, ind_size, offset);
  gltf.add_accessor_uss(qin, zone.num_quad_indices());
  ind_size = zone.get_gltf_cyl_indices_size();
  int cin = gltf.add_bufferView(buffer, ind_size, offset);
  gltf.add_accessor_uss(cin, zone.num_cyl_indices());
  if (offset % 4 != 0)
    offset += 4-(offset%4);

  for (int i=0; i<zone.size(); i++) {
    bool qp = zone.get(i).zone_shape == OKAMI_UTILS::ZONE_SHAPE_QUAD_PRISM;
    int pos_size = qp ? zone.get_gltf_quad_position_size() : zone.get_gltf_cyl_position_size();
    int p = gltf.add_bufferView(buffer, pos_size, offset);
    gltf.add_accessor_fv3(p, qp ? zone.num_quad_coordinates() : zone.num_cyl_coordinates(), zone.get_constraints(i));

    string name = fmt::format("{}.{:02x}", path.filename().string(), zone.get(i).effect_index);

    int material = MATERIAL_UNKNOWN;
    switch (zone.get(i).entry_type) {
      case OKAMI_UTILS::ZONE_BIT_FLAG_TYPE: material = MATERIAL_BIT_FLAG; break;
      case OKAMI_UTILS::ZONE_EXIT_TYPE: material = MATERIAL_EXIT; break;
      case OKAMI_UTILS::ZONE_EXAMINE_TYPE: material = MATERIAL_EXAMINE; break;
      case OKAMI_UTILS::ZONE_ISSUN_INDICATOR_TYPE: material = MATERIAL_ISSUN_INDICATOR; break;
    }

    int mesh_index = gltf.add_mesh(name, p, -1, qp ? qin : cin, material);
    gltf.add_node(mesh_index, name);
  }

  fs::path bin(out_dir);
  bin /= bin_short;
  ofstream fout(bin, ios::out|ios::binary);
  zone.dump_gltf_binary(fout);
  fout.close();
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Expect one argument: json config file." << endl;
    return -1;
  }

  fs::path config(argv[1]);
  GLTFConfig conf;
  read_config(config, conf);

  for (vector<GLTFLevel>::iterator lvl=conf.levels.begin(); lvl!=conf.levels.end(); lvl++) {
    OKAMI_UTILS::GLTF gltf;
    // Copy materials.
    rj::Value& mats = conf.materials["materials"];
    for (int i=0; i<mats.Size(); i++)
      gltf.add_material(mats[i]);

    fs::path output_dir(conf.output_dir);
    output_dir /= lvl->name;
    fs::create_directories(output_dir);
    fs::path gltf_file(output_dir);
    gltf_file /= lvl->id;
    gltf_file += ".gltf";
    for (vector<GLTFSource>::iterator src=conf.sources.begin(); src!=conf.sources.end(); src++) {
      fs::path input_dir(lvl->input_dir);
      input_dir /= lvl->id;
      input_dir += src->archive;
      input_dir += "_dir";

      if (!fs::exists(input_dir) || !fs::is_directory(input_dir))
        continue;

      fs::path input_file;
      for (auto const& entry : fs::directory_iterator(input_dir)) {
        if (fs::is_regular_file(entry) && entry.path().extension() == src->extension)
          input_file = entry;
      }

      if (input_file == "") {
        cerr << "No " << src->extension << " file for " << lvl->id << ". Skipping." << endl;
        continue;
      }

      switch (src->type) {
        case GLTFAKT_TYPE:  add_akt(gltf, input_file, output_dir); break; // add_akt
        case ZONE_TYPE: add_zone(gltf, input_file, output_dir); break;
        default: cerr << "Unknown source type: " << src->type << endl; continue;
      }
    }

    ofstream fout(gltf_file, ios::out|ios::binary);
    gltf.write(fout);
    fout.close();
  }

  return 0;
}