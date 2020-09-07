#include <filesystem>
#include <iostream>
#include <fstream>
using namespace std;
namespace fs = filesystem;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Need .AKT file arg." << endl;
    return -1;
  }

  fs::path path(argv[1]);

  if (!fs::exists(path) || path.extension() != ".AKT") {
    cerr << "Are you fucking with me?" << endl;
    return -1;
  }

  fs::path out_dir(path);
  out_dir += "_dir";
  fs::create_directory(out_dir);

  uint32_t num_files = 0;
  // fuck it I don't care about extensions. AK for everyone.

  ifstream fin(path, ios::in|ios::binary);
  fin.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
  
  uint32_t* offsets = (uint32_t*)calloc(num_files, sizeof(uint32_t));
  fin.read(reinterpret_cast<char*>(offsets), num_files*sizeof(uint32_t));

  size_t file_size = fs::file_size(path);
  for (int i=0; i<num_files; i++) {
    fs::path out(out_dir);
    char num[3];
    out /= itoa(i, num, 10);
    out += ".AK";

    size_t size = (i == num_files - 1) ? (file_size - offsets[i]) : (offsets[i+1] - offsets[i]);
    char* buffer = (char*)calloc(size, sizeof(char));
    fin.seekg(offsets[i], ios::beg);
    fin.read(buffer, size);

    ofstream fout(out, ios::out|ios::binary);
    fout.write(buffer, size);
    fout.close();

    free(buffer);
  }
  fin.close();

  return 0;
}