#include <iostream>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <okami-utils/ect.h>
using namespace std;

int main(int argc, char** argv) {
  YAML::Node enemies = YAML::LoadFile("data/enemy_id.yaml");

  OKAMI_UTILS::ECT ect;
  ect.parse_file(argv[1]);

  for (int i=0; i<ect.size(); i++) {
    OKAMI_UTILS::ECTEntry entry=ect.get(i);
    cout << fmt::format("({}, {}, {})", (float)entry.coordinates.x, (float)entry.coordinates.y, (float)entry.coordinates.z) << endl;
    // This isn't complete and I want to push to github I guess.
    // switch (entry.flags & ECT_FLAGS_TOD_MASK) {
    //   case ECT_FLAGS_TOD_DAY_ONLY: cout << "Day only, "; break;
    //   case ECT_FLAGS_TOD_NIGHT_ONLY: cout << "Night only, "; break;
    //   default: cout << "Anytime, ";
    // };
    // switch (entry.flags & ECT_FLAGS_ESCAPE_MASK) {
    //   case ECT_FLAGS_INESCAPABLE: cout << "inescapable" << endl; break;
    //   default: cout << "escapable" << endl;
    // }
    for (int j=0; j<4; j++) {
      if (entry.enemy[j].total == 0)
        continue;
      
      cout << dec << unsigned(entry.enemy[j].total) << " ";
      if (enemies[(int)entry.enemy[j].enemy_id])
        cout << enemies[(int)entry.enemy[j].enemy_id] << endl;
      else
        cout << "Unknown (" << hex << setfill('0') << setw(2) << unsigned(entry.enemy[j].enemy_id) << ")" << endl;
    }
  }

  return 0;
}