#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
namespace fs = filesystem;

static const char ak[] = {'A', 'K', '\0', '\0'};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Didn't supply args: source_dir target_akt.";
    return -1;
  }

  fs::path src(argv[1]);
  fs::path target(argv[2]);

  if (!fs::exists(src) || !fs::is_directory(src)) {
    cerr << "Are you shitting me?" << endl;
    return -1;
  }

  vector<fs::path> files;

  for (fs::directory_iterator itr(src), end; itr != end; itr++) {
    if (itr->is_regular_file() && itr->path().extension() == ".AK")
      files.push_back(itr->path());
  }

  ofstream fout(target, ios::out|ios::binary);
  uint32_t count = files.size();
  fout.write(reinterpret_cast<char*>(&count), sizeof(count));

  // This is how the first offset is determined. Don't ask me why.
  uint32_t first_offset = 0x68+(count/4)*32;
  uint32_t offset = first_offset;
  for (vector<fs::path>::iterator it=files.begin(); it!=files.end(); it++) {
    fout.write(reinterpret_cast<char*>(&offset), sizeof(offset));
    offset += fs::file_size(*it);
  }

  for (vector<fs::path>::iterator it=files.begin(); it!=files.end(); it++)
    fout.write(ak, sizeof(ak));

  int pos = fout.tellp();
  char* padding = (char*)calloc(32, sizeof(char));
  while (pos < first_offset) {
    fout.write(padding, pos + 32 < first_offset ? 32 : first_offset-pos);
    pos += 32;
  }

  for (vector<fs::path>::iterator it=files.begin(); it!=files.end(); it++) {
    int size = fs::file_size(*it);
    char* buffer = (char*)calloc(size, sizeof(char));
    ifstream fin(*it, ios::in|ios::binary);
    fin.read(buffer, size);
    fin.close();
    fout.write(buffer, size);
    free(buffer);
  }

  pos = fout.tellp();
  int pad = pos % 32;
  if (pad != 0)
    fout.write(padding, 32-pad);
  fout.close();

  return 0;
}