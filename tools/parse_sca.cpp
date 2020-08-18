#include <iostream>
#include <fstream>
#include <string>
//#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <okami-utils/sca.h>
using namespace std;

int main(int argc, char *argv[]) {  
  if (argc == 1) {
    cerr << "Didn't supply .SCA file.";
    return -1;
  }

  char* path = argv[1];
  string path_string(path);
  size_t ext_pos = path_string.rfind('.');
  string region = path_string.substr(ext_pos-3, 1);
  string area = path_string.substr(ext_pos-2, 2);
  uint8_t region_id = stoi(region, 0, 16);
  uint8_t area_id = stoi(area, 0, 16);
  cout << hex << unsigned(region_id) << unsigned(area_id) << endl;
  YAML::Node world = YAML::LoadFile("data/area_id.yaml");
  if (world[region_id][area_id])
    cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;

  OKAMI_UTILS::SCA sca(path);

  int size = sca.size();
  for (int i=0;i<sca.size();i++) {
    OKAMI_UTILS::SCAEntry entry = sca.get(i);
    // cout << i << " pre_entry: " << hex << entry.pre_entry << endl;
    // cout << i << " header: " << hex << entry.header << endl;
    cout << i << " y: " << entry.y << endl;
    cout << i << " height: " << entry.height << endl;
    cout << i << " uf: " << entry.unknown_float << endl;
    cout << i << " x1: " << entry.p1.x << endl;
    cout << i << " z1: " << entry.p1.z << endl;
    cout << i << " x2: " << entry.p2.x << endl;
    cout << i << " z2: " << entry.p2.z << endl;
    cout << i << " x3: " << entry.p3.x << endl;
    cout << i << " z3: " << entry.p3.z << endl;
    cout << i << " x4: " << entry.p4.x << endl;
    cout << i << " z4: " << entry.p4.z << endl;
    // cout << i << " u2: " << hex << unsigned(entry.unknown2) << endl;
    // cout << i << " entry_type: " << hex << unsigned(entry.entry_type) << endl;
    // cout << i << " effect_index: " << hex << entry.effect_index << endl;
    // cout << i << " u3: " << hex << unsigned(entry.unknown3) << endl;
    // cout << i << " u4: " << hex << unsigned(entry.unknown4) << endl;
    // cout << i << " u5: " << hex << unsigned(entry.unknown5) << endl;
    // cout << i << " u6: " << hex << unsigned(entry.unknown6) << endl;

    // for (int n=0;n<112;n++) {
    //   if (entry.unknown_data[n] != 0)
    //     cout << i << "[" << n << "] not zero: " << hex << unsigned(entry.unknown_data[n]) << endl;
    // }
  }

  return 0;
}