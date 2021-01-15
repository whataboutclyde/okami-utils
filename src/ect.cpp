#include <iostream>
#include <fmt/format.h>
#include <okami-utils/ect.h>
using namespace std;

namespace OKAMI_UTILS {

bool ECT::process_file(ifstream& fin) {
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

    // cout << "unknown1: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown1) << endl;
    // cout << "flags: " << hex << setfill('0') << setw(2) << unsigned(entry.flags) << endl;
    // cout << "type: " << hex << setfill('0') << setw(2) << unsigned(entry.type) << endl;
    // cout << "index: " << hex << setfill('0') << setw(4) << unsigned(entry.index) << endl;
    // cout << "unknown3: " << hex << setfill('0') << setw(4) << unsigned(entry.unknown3) << endl;

    // if (entry.flags.unknownb1o7==1) {
    //   if (entry.flags.devil_gate)
    //     cerr << "Gate" << endl;
    //   else if (entry.flags.demon_scroll)
    //     cerr << "Scroll" << endl;
    //   else 
    //     cerr << "idfk" << endl;
    // }

    if (!entry.flags.devil_gate && !entry.flags.demon_scroll && !entry.flags.inescapable2)
      cerr << "huh?" << endl;

    if (entry.flags.devil_gate && !entry.flags.inescapable2)
      cerr << "WEIRD GATE" << endl;

    for (int i=0; i<4; i++) {
    //   cout << "enemy.enemy_id: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].enemy_id) << endl;
    //   cout << "enemy.cat_id: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].cat_id) << endl;
    //   for (int j=0; j<6; j++)
    //     cout << "enemy.unknowns[" << dec << j << "]: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknowns[j]) << endl;
    //   cout << "enemy.total: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].total) << endl;
    //   cout << "enemy.max_per_wave: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].max_per_wave) << endl;
    //   cout << "enemy.unknown1: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknown1) << endl;
    //   cout << "enemy.unknown2: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].unknown2) << endl;
      for (int k=0; k<12; k++) {
        if (entry.enemy[i].padding[k] != 0)
          cout << "FUCK enemy[" << dec << i << "].padding[" << dec << k << "]: " << hex << setfill('0') << setw(2) << unsigned(entry.enemy[i].padding[k]) << endl;
      }
    }
    // cout << "unknown4: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown4) << endl;
    // cout << "unknown5: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown5) << endl;
    // cout << "unknown6: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown6) << endl;
    // cout << "unknownugh: " << hex << setfill('0') << setw(2) << unsigned(entry.unknownugh) << endl;
    // cout << "unknown7: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown7) << endl;
    // cout << "unknown8: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown8) << endl;
    // cout << "unknown9: " << hex << setfill('0') << setw(2) << unsigned(entry.unknown9) << endl;
    // cout << "unknowna: " << hex << setfill('0') << setw(2) << unsigned(entry.unknowna) << endl;
    // cout << "mix_enemies: " << hex << setfill('0') << setw(2) << unsigned(entry.mix_enemies) << endl;
    for (int p=0; p<7; p++) {
      if (entry.padding[p] != 0)
        cout << "FUCK padding[" << dec << p << "]: " << hex << setfill('0') << setw(2) << unsigned(entry.padding[p]) << endl;
    }

    entries.push_back(entry);
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

  //cout << dec << count << " entries." << endl;

  return true;
}

int ECT::size() {
  return entries.size();
}

ECTEntry ECT::get(int i) {
  ECTEntry e(entries[i]);
  return e;
}

} // namespace OKAMI_UTILS