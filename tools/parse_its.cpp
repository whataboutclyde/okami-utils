#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <string>
#include <yaml-cpp/yaml.h>
#include <okami-utils/its.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply .ITS file.";
    return -1;
  }

  char* path = argv[1];
  OKAMI::ITS its(path);

  string path_string(path);
  size_t ext_pos = path_string.find(".ITS");
  string region = path_string.substr(ext_pos-3, 1);
  string area = path_string.substr(ext_pos-2, 2);
  int region_id = stoi(region, 0, 16);
  int area_id = stoi(area, 0, 16);
  cout << hex << unsigned(region_id) << endl << unsigned(area_id) << endl;
  YAML::Node world;
  try {
    world = YAML::LoadFile("data/area_id.yaml");
  } catch(std::exception &e) {
    cerr << "Failed to load yaml file" << endl;
    return -1;
  }

  if (world[region_id][area_id])
    cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;
  else
    cout << "fuck" << endl;


  YAML::Node items = YAML::LoadFile("data/item_id.yaml");
  YAML::Node boxes = YAML::LoadFile("data/container_type_id.yaml");
  YAML::Node box_state = YAML::LoadFile("data/container_state_id.yaml");
  //boxes.remove(boxes[0x5a]);
  for (int i=0; i<its.size(); i++) {
    OKAMI::ITSEntry entry = its.get(i);
    // Use this if I want to filter by commenting out stuff in the yaml files.
    // if (!boxes[entry.container_type] || !items[entry.contents_id])
    //   continue;

    //Pretty printing shit.
    if (boxes[(int)entry.container_type])
      cout << boxes[(int)entry.container_type] << ": ";
    else
      cout << "Unexpected container: " << std::hex << unsigned(entry.container_type) << ": ";
    
    if (items[(int)entry.contents_id]) {
      cout << items[(int)entry.contents_id];
      string contents_str = items[(int)entry.contents_id].as<string>();
      if (contents_str.compare("Empty") == 0 && entry.contents_id != 0x5a)
        cout << " (" << std::hex << unsigned(entry.contents_id) << ")";
      else if (contents_str.compare("Praise") == 0)
        cout << " (" << unsigned(entry.unknowns[7]) << ")";
      cout << " (";
      if (box_state[(int)entry.container_state] && strncmp(box_state[(int)entry.container_state].as<string>().c_str(), "Unknown", 8) != 0)
        cout << box_state[(int)entry.container_state];
      else
        cout << "container_state: " << std::hex << unsigned(entry.container_state);
      cout << ")" << endl;
   }
  }
}
