#define RAPIDJSON_HAS_STDSTRING 1

#include <iostream>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <okami-utils/ect.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
using namespace rapidjson;

int main(int argc, char** argv) {
  YAML::Node enemy_list = YAML::LoadFile("data/enemy_id.yaml");
  YAML::Node world = YAML::LoadFile("data/area_id.yaml");

  Document doc(kArrayType);
  Document::AllocatorType& al = doc.GetAllocator();

  for (auto& p: fs::recursive_directory_iterator("f:\\okami")) {
    if (!p.is_regular_file() ||
        p.path().extension() != ".ECT" ||
        p.path().string().find("_f")!=string::npos ||
        p.path().string().find("_g")!=string::npos || 
        p.path().string().find("_j")!=string::npos)
        continue;

    OKAMI_UTILS::ECT ect;
    ect.parse_file(p.path());

    string path_string(p.path().string());
    size_t ext_pos = path_string.find(".ECT");
    string region = path_string.substr(ext_pos-3, 1);
    string area = path_string.substr(ext_pos-2, 2);
    unsigned int region_id = stoi(region, 0, 16);
    unsigned int area_id = stoi(area, 0, 16);

    if (region_id == 0x0)
      continue;

    Value ectfile(kObjectType);
    ectfile.AddMember("region", region, al);
    ectfile.AddMember("area", area, al);
    if (world[region_id][area_id])
      ectfile.AddMember("areaName", world[region_id][area_id].as<string>(), al);
    
    Value encounters(kArrayType);
    for (int i=0; i<ect.size(); i++) {
      OKAMI_UTILS::ECTEntry entry=ect.get(i);

      Value encounter(kObjectType);
      encounter.AddMember("index", entry.index, al);
      Value coords(kArrayType);
      coords.PushBack(entry.coordinates.x, al);
      coords.PushBack(entry.coordinates.y, al);
      coords.PushBack(entry.coordinates.z, al);
      encounter.AddMember("coordinates", coords, al);
      encounter.AddMember("escapeRadius", entry.escape_radius, al);

      if (entry.flags.demon_scroll)
        encounter.AddMember("type", "Demon Scroll", al);
      else if (entry.flags.devil_gate)
        encounter.AddMember("type", "Devil Gate", al);
      else
        encounter.AddMember("type", "No Indicator", al);

      if (entry.flags.day_only)
        encounter.AddMember("tod", "Day", al);
      else if (entry.flags.night_only)
        encounter.AddMember("tod", "Night", al);
      else
        encounter.AddMember("tod", "Anytime", al);

      encounter.AddMember("inescapable", (bool)(entry.flags.inescapable|entry.flags.inescapable2), al);
      encounter.AddMember("unrepeatable", (bool)entry.flags.unrepeatable, al);
      encounter.AddMember("separateWaves", (bool)entry.separate_waves, al);
    
      Value enemies(kArrayType);
      for (int j=0; j<4; j++) {
        if (entry.enemy[j].total == 0)
          continue;

        Value enemy(kObjectType);
        enemy.AddMember("total", entry.enemy[j].total, al);
        enemy.AddMember("maxConcurrent", entry.enemy[j].max_concurrent, al);
        if (enemy_list[(int)entry.enemy[j].enemy_id])
          enemy.AddMember("name", enemy_list[(int)entry.enemy[j].enemy_id].as<string>(), al);
        else
          enemy.AddMember("name", "Unknown", al);
        enemies.PushBack(enemy, al);
      }
      encounter.AddMember("enemies", enemies, al);
      encounters.PushBack(encounter, al);
    }
    ectfile.AddMember("encounters", encounters, al);
    doc.PushBack(ectfile, al);
  }

  StringBuffer json;
  Writer<StringBuffer> writer(json);
  doc.Accept(writer);
  std::cout << json.GetString();

  return 0;
}