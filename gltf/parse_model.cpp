#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <okami-utils/model.h>
#include "gltf.h"
using namespace std;
namespace fs = filesystem;

int main(int argc, char *argv[]) {  
  if (argc == 1) {
    cerr << "Didn't supply .SCR file.";
    return -1;
  }

  OKAMI_UTILS::Model m;
  fs::path path(argv[1]);
  m.parse_file(path);

  OKAMI_UTILS::GLTF gltf;
  
  for (int i=0; i<m.size(); i++) {
    //int mesh_index = gltf.add_mesh(path.stem())
    //gltf.add_node()
  }

  ofstream fout("model_test.gltf", ios::out|ios::binary);
  gltf.write(fout);
  fout.close();
}