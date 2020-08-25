#include <iostream>
#include <filesystem>
#include <okami-utils/akt.h>
using namespace std;
namespace fs = filesystem;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Didn't supply .AKT file." << endl;
    return -1;
  }

  fs::path path(argv[1]);
  if (!fs::is_regular_file(path)) {
    cerr << path << " is not a regular file." << endl;
    return -1;
  }

  fs::path out_path = path.filename();
  out_path += ".obj";
  ofstream fout(out_path, ios::out);
  if (!fout.is_open()) {
    cerr << "Can't write file: " << out_path << endl;
    return -1;
  }

  OKAMI_UTILS::AKT akt(argv[1]);

//  int count_coords = 0;
  for (int i=0; i<akt.size(); i++) {
    OKAMI_UTILS::AK data = akt.get(i);
    int coord_count = data.num_coords();
    OKAMI_UTILS::AKCoordinateEntry* coords = data.get_coords();

//    cout << "coords for " << i << ": " << coord_count << endl;
    for (int n=0; n<coord_count; n++)
      fout << "v " << (float)coords[n].x << " " << (float)coords[n].y << " " << (float)coords[n].z << endl;
//    count_coords += coord_count;
  }
//  cout << "total coords: " << count_coords << endl;

  int index_offset = 1;
  for (int i=0; i<akt.size(); i++) {
    OKAMI_UTILS::AK data = akt.get(i);
    int index_count = data.num_triangles();
    uint16_t* indices = data.get_triangles();

    for (int n=0; n<index_count; n++)
      fout << "f " << indices[3*n]+index_offset << " " << indices[3*n+1]+index_offset << " " << indices[3*n+2]+index_offset << endl;

    index_offset += data.num_coords();
  }

  return 0;
}