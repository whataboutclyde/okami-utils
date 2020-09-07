#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <iomanip>
#include <string>
//#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <okami-utils/ans.h>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply .ANS file.";
    return -1;
  }

  char* path = argv[1];
  OKAMI_UTILS::ANS ans(path);

  string path_string(path);
  size_t ext_pos = path_string.find(".ANS");
  string region = path_string.substr(ext_pos-3, 1);
  string area = path_string.substr(ext_pos-2, 2);
  unsigned int region_id = stoi(region, 0, 16);
  unsigned int area_id = stoi(area, 0, 16);

  YAML::Node world = YAML::LoadFile("data/area_id.yaml");
  if (world[region_id][area_id])
    cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;

  YAML::Node animals = YAML::LoadFile("data/animal_id.yaml");

  list<uint8_t> keys = ans.get_keys();

  for (list<uint8_t>::iterator it=keys.begin(); it!=keys.end(); it++)
  {
    vector<OKAMI_UTILS::ANSEntry> group = ans.get(*it);
    cout << "Group " << unsigned(*it) << " (" << group.size() << "): ";
    for (int i=0; i<group.size(); i++) {
      unsigned int an_id = group[i].an_id;
      if (animals[an_id])
        cout << animals[an_id];
      else
        cout << "Unknown (" << hex << an_id << ")";
      switch(group[i].tod) {
        case OKAMI_UTILS::TOD_DAY_ONLY:   cout << " (D)"; break;
        case OKAMI_UTILS::TOD_NIGHT_ONLY: cout << " (N)"; break;
        case OKAMI_UTILS::TOD_ALWAYS:     cout << " (A)"; break;
      }
      if (i!=group.size()-1)
        cout << ", ";
      else
        cout << " (" << (float)group[i].coordinates.x << ", " << (float)group[i].coordinates.y << ", " << (float)group[i].coordinates.z << ")" << endl;

      //cout << "animal: " << animals[group[i].an_id] << endl;

      // if (group[i].an_id != group[i].an_id2)
      //   cerr << "bad assumption an_id2: " << hex << setfill('0') << setw(2) << unsigned(group[i].an_id2) << " not " << hex << setfill('0') << setw(2) << unsigned(group[i].an_id) << endl;

      // cout << "unknown1: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown1) << endl;
      // cout << "unknown2: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown2) << endl;
      // cout << "unknown3: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown3) << endl;
      // cout << "unknown4: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown4) << endl;
      // cout << "unknown5: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown5) << endl;
      // cout << "unknown6: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown6) << endl;
      // cout << "unknown7: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown7) << endl;
      // cout << "unknown8: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown8) << endl;
      // cout << "unknown9: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknown9) << endl;
      // cout << "unknowna: " << hex << setfill('0') << setw(2) << unsigned(group[i].unknowna) << endl;

      // for (int n=0;n<ANS_ENTRY_PADDING_SIZE;n++) {
      //   if (group[i].padding[n] != 0) 
      //     cout << animals[group[i].an_id] << " entry " << i << " index " << n << " padding not 00: " << hex << setfill('0') << setw(2) << unsigned(group[i].padding[n]) << endl;
      // }
    }
    //cout << endl;
  }
}
