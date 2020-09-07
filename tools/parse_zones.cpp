#include <iostream>
#include <fstream>
#include <string>
//#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <okami-utils/zone.h>
using namespace std;

int main(int argc, char *argv[]) {  
  if (argc == 1) {
    cerr << "Didn't supply Zone (.MEH|.SCA|.SCI) file.";
    return -1;
  }

  char* path = argv[1];
  string path_string(path);
  // size_t ext_pos = path_string.rfind('.');
  // string region = path_string.substr(ext_pos-3, 1);
  // string area = path_string.substr(ext_pos-2, 2);
  // int region_id = stoi(region, 0, 16);
  // int area_id = stoi(area, 0, 16);
  // YAML::Node world = YAML::LoadFile("data/area_id.yaml");
  // if (world[region_id][area_id])
  //   cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;

  OKAMI_UTILS::Zone zone(path);
  //cout << fs::path(path).filename().string() << endl;

  int size = zone.size();
  for (int i=0;i<zone.size();i++) {
    OKAMI_UTILS::ZoneEntry entry = zone.get(i);

    if (entry.pre_entry!=0)
      cerr << "fuck not zero " << hex << entry.pre_entry << endl;

    if (entry.always01 != 1)
      cerr << "always01: " << hex << unsigned(entry.always01) << " " << path << endl;

    if (entry.zone_shape != 1 && entry.zone_shape != 2)
      cerr << "zone_shape: " << hex << unsigned(entry.always01) << " " << path << endl;

    if (entry.zero1 != 0)
      cerr << "zero1: " << hex << unsigned(entry.zero1) << " " << path << endl;

    if (entry.zero2 != 0)
      cerr << "zero2: " << hex << unsigned(entry.zero2) << " " << path << endl;

    if (entry.zone_shape == 1 &&
        entry.p2.x == entry.p3.x && entry.p3.x == entry.p4.x &&
        entry.p2.z == entry.p3.z && entry.p3.z == entry.p4.z)
      cerr << "FUCK 01"<<path <<endl;

    if (entry.zone_shape == 2 &&
        !(entry.p2.x == entry.p3.x && entry.p3.x == entry.p4.x) &&
        !(entry.p2.z == entry.p3.z && entry.p3.z == entry.p4.z))
      cerr << "02 "<<path << ": " << hex << setfill('0') << setw(2) << unsigned(entry.effect_index) << endl;

    //cout << hex << setfill('0') << setw(2) << unsigned(entry.effect_index) << ": " << entry.unknown_float << endl;

    // cout << i << " pre_entry: " << hex << entry.pre_entry << endl;
    // cout << i << " header: " << hex << entry.header << endl;
    // cout << i << " y: " << entry.y << endl;
    // cout << i << " height: " << entry.height << endl;
    // cout << i << " uf: " << entry.unknown_float << endl;
    // cout << i << " x1: " << entry.p1.x << endl;
    // cout << i << " z1: " << entry.p1.z << endl;
    // cout << i << " x2: " << entry.p2.x << endl;
    // cout << i << " z2: " << entry.p2.z << endl;
    // cout << i << " x3: " << entry.p3.x << endl;
    // cout << i << " z3: " << entry.p3.z << endl;
    // cout << i << " x4: " << entry.p4.x << endl;
    // cout << i << " z4: " << entry.p4.z << endl;
    //cout << i << " u2: " << hex << unsigned(entry.unknown2) << endl;
    //cout << setfill('0') << setw(2) << entry.effect_index << " entry_type: " << hex << unsigned(entry.entry_type) << endl;
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