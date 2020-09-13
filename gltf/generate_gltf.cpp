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
#include <okami-utils/model.h>
#include "gltf.h"
using namespace std;
namespace fs = filesystem;
namespace rj = rapidjson;

#define FILELIST_NAME "filelist.lst"

enum FILE_TYPE {
  AKT_TYPE   = 0,
  ZONE_TYPE  = 1,
  SCP_TYPE   = 2   // Going to assume it's been unpacked for now anyway.
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
  bool unpacked;
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
      if (sub[i].HasMember("unpacked"))
        gs.unpacked = sub[i]["unpacked"].GetBool();
      else
        gs.unpacked = false;
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
  int parent_node = gltf.add_parent_node("Collision");

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

    string name = fmt::format("{}.AK.{:02x}", path.stem().string(), i);
    int mesh_index = gltf.add_mesh(name, p, n, in, -1, -1, MATERIAL_COLLISION);
    gltf.add_node(mesh_index, name, parent_node);
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

  int file_parent_node = -1;
  // Ideally I could have an array indexed with the enum, but then I need to know all the values...
  // So for now we're doing it like this.
  int exit_parent_node = -1;
  int bit_flag_parent_node = -1;
  int examine_parent_node = -1;
  int indicator_parent_node = -1;

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
    int parent_node = file_parent_node;
    bool qp = zone.get(i).zone_shape == OKAMI_UTILS::ZONE_SHAPE_QUAD_PRISM;
    int pos_size = qp ? zone.get_gltf_quad_position_size() : zone.get_gltf_cyl_position_size();
    int p = gltf.add_bufferView(buffer, pos_size, offset);
    gltf.add_accessor_fv3(p, qp ? zone.num_quad_coordinates() : zone.num_cyl_coordinates(), zone.get_constraints(i));

    string name = fmt::format("{}.{:02x}", path.filename().string(), zone.get(i).effect_index);
    cout << name << endl;
    int material = MATERIAL_UNKNOWN;
    switch (zone.get(i).entry_type) {
      case OKAMI_UTILS::ZONE_BIT_FLAG_TYPE: 
        material = MATERIAL_BIT_FLAG; 
        if (bit_flag_parent_node == -1)
          bit_flag_parent_node = gltf.add_parent_node("Bit Flag");
        parent_node = bit_flag_parent_node;
        break;
      case OKAMI_UTILS::ZONE_EXIT_TYPE: 
        if (exit_parent_node == -1)
          exit_parent_node = gltf.add_parent_node("Exit");
        parent_node = exit_parent_node;
        material = MATERIAL_EXIT; 
        break;
      case OKAMI_UTILS::ZONE_EXAMINE_TYPE: 
        material = MATERIAL_EXAMINE; 
        if (examine_parent_node == -1)
          examine_parent_node = gltf.add_parent_node("Examine");
        parent_node = examine_parent_node;
        break;
      case OKAMI_UTILS::ZONE_ISSUN_INDICATOR_TYPE: 
        material = MATERIAL_ISSUN_INDICATOR; 
        if (indicator_parent_node == -1)
          indicator_parent_node = gltf.add_parent_node("Indicator");
        parent_node = indicator_parent_node;
        break;
    }

    int mesh_index = gltf.add_mesh(name, p, -1, qp ? qin : cin, -1, -1, material);
    if (parent_node == -1)
      parent_node = file_parent_node = gltf.add_parent_node(path.filename().string());
    gltf.add_node(mesh_index, name, parent_node);
  }

  fs::path bin(out_dir);
  bin /= bin_short;
  ofstream fout(bin, ios::out|ios::binary);
  zone.dump_gltf_binary(fout);
  fout.close();
}

// int add_textures(OKAMI_UTILS::GLTF& gltf, fs::path path, fs::path out_dir) {
//   // First we need filelist to make sure they're in the right order...
//   fs::path fl(path);
//   fl /= FILELIST_NAME;

//   int tex_offset = -1;
//   string line;
//   ifstream fin(fl, ios::in);
//   getline(fin, line);
//   int count = atoi(line.c_str());
//   for (int i=0; i<count; i++) {
//     getline(fin, line);
//     if (line.rfind(".DDS") != string::npos) {
//       int ai = gltf.add_image(line);
//       gltf.add_texture(ai);
//       if (i == 0)
//         tex_offset = ai;
//       fs::path from(path);
//       from /= line;
//       fs::path to(out_dir);
//       to /= from.filename();
//       //This is throwing errors if the files already exist and I don't want to deal with it right now.
//       //fs::copy(from, to, fs::copy_options::update_existing);
//     }
//   }

//   return tex_offset;
// }

// void add_scp(OKAMI_UTILS::GLTF& gltf, fs::path path, fs::path out_dir) {
//   int parent_node = gltf.add_parent_node("Scenery");
//   fs::path scp_dir(path);
//   scp_dir += "_dir";
//   fs::path ddp_dir;

//   for (auto const& entry : fs::directory_iterator(scp_dir)) {
//     if (fs::is_directory(entry) && entry.path().extension() == ".DDP_dir") {
//       ddp_dir = entry;
//       break;
//     }
//   }

//   int tex_offset = add_textures(gltf, ddp_dir, out_dir);

//   fs::path bin(out_dir);
//   bin /= path.filename();
//   bin += ".bin";

//   ofstream out_bin(bin, ios::out|ios::binary);

//   // Going to change the buffer size once we're done...
//   int buffer = gltf.add_buffer(1, bin.string());

//   int offset = 0;
//   // int count = 0;
//   for (auto const& entry : fs::directory_iterator(scp_dir)) {
//     if (fs::is_regular_file(entry) && entry.path().extension() == ".SCR") {
//       // cout << count << endl;
//       OKAMI_UTILS::Model m;
      
//       m.parse_file(entry);
//       int model_node = gltf.add_parent_node(entry.path().filename().string(), parent_node);

//       for (int j=0; j<m.submeshes.size(); j++) {
//         string submesh_string = fmt::format("{}.{:02x}",entry.path().filename().string(),j);
//         int submesh_node = gltf.add_parent_node(submesh_string, model_node);
//         for (int k=0; k<m.submeshes[j].divisions.size(); k++) {
//           string mdiv_string = fmt::format("{}.{:02x}", submesh_string, k);
//           OKAMI_UTILS::MeshDivision& md = m.submeshes[j].divisions[k];
//           int ind_size = md.get_gltf_indices_size();
//           int ind = gltf.add_bufferView(buffer, ind_size, offset);
//           gltf.add_accessor_uss(ind, md.num_indices());
//           if (offset%4 != 0)
//             offset += 4-(offset%4);
//           int pos_size = md.get_gltf_vertices_size();
//           int pos = gltf.add_bufferView(buffer, pos_size, offset);
//           gltf.add_accessor_fv3(pos, md.num_coordinates());
//           // int texc_size = md.get_gltf_itm_size();
//           // int texc = gltf.add_bufferView(buffer, texc_size, offset);
//           // gltf.add_accessor_ubv4(texc, md.num_coordinates());
//           // int tcw_size = md.get_gltf_tcw_size();
//           // int tcw = gltf.add_bufferView(buffer, tcw_size, offset);
//           // gltf.add_accessor_ubv4(tcw, md.num_coordinates());
//           //int mesh_index = gltf.add_mesh(mdiv_string, pos, -1, ind, texc, tcw, -1, md.header.texture_index);
//           int mesh_index = gltf.add_mesh(mdiv_string, pos, -1, ind, -1, -1, MATERIAL_COLLISION, -1);
//           gltf.add_node(mesh_index, mdiv_string, submesh_node);
//         }
//       }
//       m.dump_gltf_binary(out_bin);
//       // if (++count>1)
//       //   break;
//     }
//   }

//   // I hate I'm doing this.
//   gltf.change_buffer_length(buffer, offset);
//   out_bin.close();
// }

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
        case AKT_TYPE:  add_akt(gltf, input_file, output_dir); break; 
        case ZONE_TYPE: add_zone(gltf, input_file, output_dir); break;
        // case SCP_TYPE:  
        //   if (!src->unpacked) {
        //     cerr << "I'm not supporting packed SCP right now because haha layered archives." << endl;
        //     continue;
        //   }
        //   add_scp(gltf, input_file, output_dir); 
        //   break;
        default: cerr << "Unknown source type: " << src->type << endl; continue;
      }
    }

    ofstream fout(gltf_file, ios::out|ios::binary);
    gltf.write(fout);
    fout.close();
  }

  return 0;
}