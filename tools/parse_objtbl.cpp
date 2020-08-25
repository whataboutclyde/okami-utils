#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <string>
#include <iomanip>
#include <yaml-cpp/yaml.h>
#include <okami-utils/objtbl.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply .OBJTBL file.";
    return -1;
  }

  char* path = argv[1];
  cout << path << endl;
  OKAMI_UTILS::OBJTBL objtbl(path);
  cout << path << endl;

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

  if (world[region_id][area_id])
    cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;
  else
    cout << "fuck" << endl;

  if (objtbl.size() == 0)
    return -1;


  // YAML::Node items = YAML::LoadFile("data/item_id.yaml");
  // YAML::Node boxes = YAML::LoadFile("data/container_type_id.yaml");
  // YAML::Node box_state = YAML::LoadFile("data/container_state_id.yaml");
  //boxes.remove(boxes[0x5a]);
  for (int i=0; i<objtbl.size(); i++) {
    OKAMI_UTILS::OBJTBLEntry entry = objtbl.get(i);

    cout << /*"cat_id: " << */hex << setfill('0') << setw(2) << unsigned(entry.cat_id); //<< endl;
    cout << " ";
    cout << /*"obj_id: " << */hex << setfill('0') << setw(2) << unsigned(entry.obj_id) << endl;
    cout << "unknown3: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown3) << endl;
    cout << "unknown4: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown4) << endl;
    // cout << "x_size: " << hex << setfill('0') << setw(2) << unsigned(entry.x_size) << endl;
    // cout << "y_size: " << hex << setfill('0') << setw(2) << unsigned(entry.y_size) << endl;
    // cout << "z_size: " << hex << setfill('0') << setw(2) << unsigned(entry.z_size) << endl;
    // cout << "x_rotate: " << hex << setfill('0') << setw(2) << unsigned(entry.x_rotate) << endl;
    // cout << "y_rotate: " << hex << setfill('0') << setw(2) << unsigned(entry.y_rotate) << endl;
    // cout << "z_rotate: " << hex << setfill('0') << setw(2) << unsigned(entry.z_rotate) << endl;
    // cout << "x_coord: " << hex << setfill('0') << setw(4) << entry.x_coord << endl;
    // cout << "y_coord: " << hex << setfill('0') << setw(4) << entry.y_coord << endl;
    // cout << "z_coord: " << hex << setfill('0') << setw(4) << entry.z_coord << endl;
    cout << "unknown5: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown5) << endl;
    cout << "unknown6: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown6) << endl;
    cout << "unknown7: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown7) << endl;
    cout << "unknown8: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown8) << endl;
    cout << "unknown9: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown9) << endl;
    cout << "unknowna: " << hex << setfill('0') << setw(2) << unsigned(entry.unknowna) << endl;
    for (int n=0; n<12; n++) {
      if (entry.unknowns[n]!=0)
        cout << "unknowns[" << dec << n << "]: " << hex << setfill('0') << setw(2) << unsigned(entry.unknowns[n]) << endl;
    }

    // Use this if I want to filter by commenting out stuff in the yaml files.
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
