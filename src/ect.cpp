#include <iostream>
#include <okami-utils/ect.h>
using namespace std;

namespace OKAMI_UTILS {

bool ECT::parse_file(char* path) {
  return parse_file(fs::path(path));
}

bool ECT::parse_file(fs::path path) {
  //cleanup();

  if (!fs::exists(path)) {
    cerr << "File does not exist: " << path << endl;
    return false;
  }

  ifstream fin(path, ios::in|ios::binary);
  if (!fin.is_open()) {
    cerr << "Couldn't open file " << path << endl;
    return false;
  }

  cout << path.string() << endl;

  bool ret = parse_file(fin);
  fin.close();
  return ret;
}

bool ECT::parse_file(ifstream& fin) {
  ECT_ENTRY_HEADER_TYPE entry_header = 0;
  ECTEntry entry;
  int count = 0;

  while (true) {
    fin.read(reinterpret_cast<char *>(&entry_header), sizeof(entry_header));
    if (entry_header == 0xFFFFFFFF)
      break;
    if (entry_header != 0x6) {
      cout << "fml" << endl;
      return false;
    }
    count++;
    fin.read(reinterpret_cast<char *>(&entry), sizeof(entry));

    cout << "unknown1: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown1) << endl;
    cout << "unknown2: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown2) << endl;
    cout << "escape: " << hex << setfill('0') << setw(2) << unsigned(entry.escape) << endl;
    cout << "type: " << hex << setfill('0') << setw(2) << unsigned(entry.type) << endl;
    cout << "index: " << hex << setfill('0') << setw(4) << unsigned(entry.index) << endl;
    cout << "unknown3: " << hex << setfill('0') << setw(4) << unsigned(entry.unknown3) << endl;
    for (int i=0; i<4; i++) {
      cout << "enemy.enemy_id: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].enemy_id) << endl;
      cout << "enemy.cat_id: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].cat_id) << endl;
      cout << "enemy.unknown3: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknown3) << endl;
      cout << "enemy.unknown4: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknown4) << endl;
      cout << "enemy.total: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].total) << endl;
      cout << "enemy.max_per_wave: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].max_per_wave) << endl;
      cout << "enemy.unknown1: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknown1) << endl;
      cout << "enemy.unknown2: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknown2) << endl;
    }
    cout << "unknown4: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown4) << endl;
    cout << "unknown5: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown5) << endl;
    cout << "unknown6: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown6) << endl;
    cout << "aggro: " << hex << setfill('0') << setw(2) << unsigned(entry.aggro) << endl;
    cout << "unknown7: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown7) << endl;
    cout << "unknown8: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown8) << endl;
    cout << "unknown9: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown9) << endl;
    cout << "unknowna: " << hex << setfill('0') << setw(2) << unsigned(entry.unknowna) << endl;
    cout << "mix_enemies: " << hex << setfill('0') << setw(2) << unsigned(entry.mix_enemies) << endl;

  }

  // Check remainder of file after 0xFFFFFFFF is zero.
  while (!fin.eof()) {
    char c;
    fin.get(c);
    if (c != 0) {
      cout << "FUCK" << endl;
      return false;
    }
  }

  cout << dec << count << " entries." << endl;

  return true;
}

} // namespace OKAMI_UTILS