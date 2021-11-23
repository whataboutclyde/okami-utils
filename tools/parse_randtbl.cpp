#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <string>
#include <iomanip>
#include <yaml-cpp/yaml.h>
#include <okami-utils/randtbl.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply .RANDTBL file.";
    return -1;
  }

  char* path = argv[1];
  // cout << path << endl;
  OKAMI_UTILS::RANDTBL randtbl(path);
  // cout << path << endl;

  string path_string(path);
  size_t ext_pos = path_string.rfind("_");
  string region = path_string.substr(ext_pos-3, 1);
  string area = path_string.substr(ext_pos-2, 2);
  int region_id = stoi(region, 0, 16);
  int area_id = stoi(area, 0, 16);
  //cout << hex << unsigned(region_id) << endl << unsigned(area_id) << endl;
  YAML::Node world;
  try {
    world = YAML::LoadFile("data/area_id.yaml");
  } catch(std::exception &e) {
    cerr << "Failed to load yaml file" << endl;
    return -1;
  }

  // if (world[region_id][area_id])
  //   cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;
  // else
  //   cout << "fuck" << endl;

  if (randtbl.size() == 0)
    return -1;


  YAML::Node items = YAML::LoadFile("data/item_id.yaml");
  // YAML::Node boxes = YAML::LoadFile("data/container_type_id.yaml");
  // YAML::Node box_state = YAML::LoadFile("data/container_state_id.yaml");
  //boxes.remove(boxes[0x5a]);
  cout << "entries: " << randtbl.size() << endl;
  for (int i=0; i<randtbl.size(); i++) {
    OKAMI_UTILS::RANDTBLGroup entry = randtbl.get(i);
    //cout << std::hex << unsigned(entry.header.unknown1) << " " << unsigned(entry.header.count) << " " << unsigned(entry.header.unknown3) << " " << unsigned(entry.header.unknown4) << endl;
    if (entry.header.count != entry.entries.size())
      cout << entry.header.count << " does not match " << entry.entries.size() << endl;

    if (entry.header.drop_chance > 100)
      cout << "drop_chance over 100: " << unsigned(entry.header.drop_chance) << endl;

    if (entry.header.unknown3 != 0x64 && entry.header.unknown3 != 0x00)
      cout << "unknown3 not 0x64 or 0x00: " << std::hex << unsigned(entry.header.unknown3) << endl;
    
    unsigned int sum=0;
    for (int j=0; j<entry.entries.size(); j++) {
      // if (entry.entries[j].cat_id != 0x0A)
      //   cout << "cat_id not 0x0A (it): " << std::hex << unsigned(entry.entries[j].cat_id) << endl;
      // if (entry.entries[j].unknown4 > 0x05)
      //   cout << "unknown4 > 0x05: " << std::hex << unsigned(entry.entries[j].unknown4) << endl;
      sum+=entry.entries[j].weight;
      //cout << items[(int)entry.entries[j].unknown1] << endl;
    }

    if (sum != entry.header.unknown3)
      cout << "I'm wrong. sum=" << sum << endl;

    //Use this if I want to filter by commenting out stuff in the yaml files.
    // if (!boxes[entry.container_type] || !items[entry.contents_id])
    //   continue;

    //Pretty printing shit.
    // if (boxes[(int)entry.container_type])
    //   cout << boxes[(int)entry.container_type] << ": ";
    // else
    //   cout << "Unexpected container: " << std::hex << unsigned(entry.container_type) << ": ";
    
    // if (items[(int)entry.contents_id]) {
    //   cout << items[(int)entry.contents_id];
    //   string contents_str = items[(int)entry.contents_id].as<string>();
    //   if (contents_str.compare("Empty") == 0 && entry.contents_id != 0x5a)
    //     cout << " (" << std::hex << unsigned(entry.contents_id) << ")";
    //   else if (contents_str.compare("Praise") == 0)
    //     cout << " (" << unsigned(entry.unknowns[7]) << ")";
    //   cout << " (";
    //   if (box_state[(int)entry.container_state] && box_state[(int)entry.container_state].as<string>().compare(0,8,"Unknown") != 0)
    //     cout << box_state[(int)entry.container_state];
    //   else
    //     cout << "container_state: " << std::hex << unsigned(entry.container_state);
    //   cout << ")" << endl;
    // }
  }
}
