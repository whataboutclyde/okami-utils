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
  uint8_t region_id = stoi(region, 0, 16);
  uint8_t area_id = stoi(area, 0, 16);
  YAML::Node world = YAML::LoadFile("data/area_id.yaml");
  if (world[region_id][area_id])
    cout << "*** " << world[region_id][area_id].as<string>() << " (r" << region << area << ") ***" << endl;
  // else
  //   return 0;


  YAML::Node animals = YAML::LoadFile("data/animal_id.yaml");

  list<uint8_t> keys = ans.get_keys();

  for (list<uint8_t>::iterator it=keys.begin(); it!=keys.end(); it++)
  {
    vector<OKAMI_UTILS::ANSEntry> group = ans.get(*it);
    // cout << "Group " << unsigned(*it) << " (" << group.size() << "): ";
    for (int i=0; i<group.size(); i++) {
      // if (animals[group[i].an_id])
      //   cout << animals[group[i].an_id];
      // else
      //   cout << "Unknown (" << hex << unsigned(group[i].an_id) << ")";
      // switch(group[i].tod) {
      //   case OKAMI_UTILS::TOD_DAY_ONLY:   cout << " (D)"; break;
      //   case OKAMI_UTILS::TOD_NIGHT_ONLY: cout << " (N)"; break;
      //   case OKAMI_UTILS::TOD_ALWAYS:     cout << " (A)"; break;
      // }
      // if (i!=group.size()-1)
      //   cout << ", ";

      //cout << "animal: " << animals[group[i].an_id] << endl;

      if (group[i].an_id != group[i].an_id2)
        cerr << "bad assumption an_id2: " << hex << setfill('0') << setw(2) << unsigned(group[i].an_id2) << " not " << hex << setfill('0') << setw(2) << unsigned(group[i].an_id) << endl;

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

      // cout << " (" << group[i].x_coord << ", " << group[i].y_coord << ", " << group[i].z_coord << ")" << endl;
      for (int n=0;n<ANS_ENTRY_PADDING_SIZE;n++) {
        if (group[i].padding[n] != 0) 
          cout << animals[group[i].an_id] << " entry " << i << " index " << n << " padding not 00: " << hex << setfill('0') << setw(2) << unsigned(group[i].padding[n]) << endl;
      }
    }
    //cout << endl;
  }
    

    //Pretty printing shit.
    // if (boxes[entry.container_type])
    //   cout << boxes[entry.container_type] << ": ";
    // else
    //   cout << "Unexpected container: " << std::hex << unsigned(entry.container_type) << ": ";
    
    // if (items[entry.contents_id]) {
    //   cout << items[entry.contents_id];
    //   string contents_str = items[entry.contents_id].as<string>();
    //   if (contents_str.compare("Empty") == 0 && entry.contents_id != 0x5a)
    //     cout << " (" << std::hex << unsigned(entry.contents_id) << ")";
    //   else if (contents_str.compare("Praise") == 0)
    //     cout << " (" << unsigned(entry.unknowns[7]) << ")";
    //   cout << " (";
    //   if (box_state[entry.container_state] && strncmp(box_state[entry.container_state].as<string>().c_str(), "Unknown", 8) != 0)
    //     cout << box_state[entry.container_state];
    //   else
    //     cout << "container_state: " << std::hex << unsigned(entry.container_state);
    //   cout << ")" << endl;
   //}
  //}
}
